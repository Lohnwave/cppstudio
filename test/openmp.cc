#include "thirdparty/glog/logging.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <iostream>

const int INSTANCE_SIZE = 1200;
const int RULE_SIZE = 45;
class Instance {
public:
    Instance() {}
    void SetTag(const std::unordered_set<std::string>& tagset) {
        for (auto iter = tagset.begin(); iter != tagset.end(); ++ iter) {
            rule_tag_.insert(*iter);
        }
    }
    bool HasTag(const std::string& tag) {
        if (rule_tag_.find(tag) != rule_tag_.end()) {
            return true;
        }
        return false;
    }
private:
    std::unordered_set<std::string> rule_tag_;
};
typedef  std::shared_ptr<Instance> InstancePtr;
class Rerank{
public:
    Rerank() {
        priority_rules_.resize(10);
        for (int i = 0; i < RULE_SIZE; ++ i ) {
            if (i % 2 == 0) {
                priority_rules_.at(0).emplace_back("搞笑");
            } else {
                priority_rules_.at(0).emplace_back("news");
            }
        }
        priority_instance_s_.resize(10);
    }
    bool rank(std::vector<InstancePtr>& instance_s, int slotNum) {
        for (int slot = 0; slot < slotNum; ++ slot) {
            validateRule1(slot, instance_s);
        }
        return true;
    }
    bool validateRule1(int slotNum, std::vector<InstancePtr>& instance_s) {
#pragma omp parallel for
        for ( int priority = 0; priority < priority_rules_.size(); ++ priority) {
            auto& rules = priority_rules_.at(priority);
            for (int rule_i = 0; rule_i < rules.size(); ++ rule_i) {
                for (int ins_i = slotNum; ins_i < INSTANCE_SIZE; ++ ins_i) {
                    auto& instance = instance_s.at(ins_i);
                    if (instance->HasTag(rules.at(rule_i)) ) {
                        priority_instance_s_.at(priority).emplace_back(instance);
                        LOG(INFO) << "add instance: " << ins_i;
                        usleep(10000);
                    }
                }
            }
        }
        return true;
    }
    bool validateRule2(int slotNum, std::vector<InstancePtr>& instance_s) {
        return true;
    }

private:
    std::vector<std::vector<std::string>> priority_rules_;
    std::vector<std::vector<InstancePtr>> priority_instance_s_;
};

bool GetInstance(
    std::unordered_set<std::string>& tagset, 
    std::vector<InstancePtr>& instance_s ) {
        int ins_size = INSTANCE_SIZE;
        for (int i = 0; i < ins_size; ++ i) {
            InstancePtr ins = std::make_shared<Instance> ();
            ins->SetTag(tagset);
            instance_s.emplace_back(ins);
            usleep(1000);
        }
}

int main()
{
    std::unordered_set<std::string> tagset;
    tagset.insert("news");
    tagset.insert("搞笑");
    tagset.insert("vedio");

    std::vector<InstancePtr> instance_s;
    GetInstance(tagset, instance_s);

    int slotNum = 10;
    while(1) {
        Rerank rerank;
        rerank.rank(instance_s, slotNum);
    }



    return 0;
}
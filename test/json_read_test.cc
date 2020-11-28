#include "thirdparty/jsoncpp/json.h"
#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"
#include <iostream>
#include <fstream>
#include <string>
#include <syslog.h>

using namespace Json;

TEST(json, test1) {
    Json::Reader read;
	Json::Value value;
    std::string confpath = "./conf/ruleconfs.json";
    std::ifstream input(confpath, std::ios::binary);
    bool is_open = input.is_open();
    EXPECT_TRUE(is_open);
    bool is_parse = read.parse(input, value);
    EXPECT_TRUE(is_parse);
    
    if (is_parse) {
        Json::Value buckets = value["buckets"];
        for (int i = 0; i < buckets.size(); ++ i) {
            int  rule_id = buckets[i]["ruleId"] .asInt();
            int  priority = buckets[i]["priority"].asInt();

            std::cout << "bucket: " << i << std::endl;
            std::cout << "\trule_id: " << rule_id << " priority: " << priority << std::endl;
            Json::Value state_s = buckets[i]["statusfilter"];
            // 读取 不定 key value
            Json::Value::Members members = state_s.getMemberNames(); 
            for (auto mem_i = members.begin(); mem_i != members.end(); ++ mem_i) {
                std::string state_key = *mem_i;
                std::string state_value = state_s[state_key].asString();
                std::cout << "\t state_key: " << state_key << "  state_value: " << state_value << std::endl;
            }
        }
    }
    input.close();
    // EXPECT_TRUE(input.close());
}
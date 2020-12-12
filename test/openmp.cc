#include "thirdparty/glog/logging.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <iostream>

class instance {
public:
    void SetTag() {
            
    }
private:
    std::unordered_set<std::string> rule_tag_;
};
typedef  std::shared_ptr<instance> InstancePtr;

bool validateRule(int slotNum, std::vector<InstancePtr>) {

}
int main()
{


    return 0;
}
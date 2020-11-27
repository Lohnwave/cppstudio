#include "thirdparty/jsoncpp/json.h"
#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace Json;
TEST(json, test1) {
    Json::Reader read;
	Json::Value value;
    std::string confpath = "./conf/ruleconfs.json";
    std::ifstream input(confpath, std::ios::binary);

    EXPECT_TRUE(input.is_open());

}
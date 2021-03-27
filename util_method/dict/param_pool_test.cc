/*
 * @Author: Lohnwave
 * @Date: 2021-03-27 13:10:24
 * @LastEditors: Lohnwave
 * @LastEditTime: 2021-03-27 14:30:54
 * @Descripttion: 
 * @version: 
 */
#include "util_method/dict/param_pool.h"
#include "thirdparty/gtest/gtest.h"
#include "thirdparty/muduo/base/Logging.h"
#include <iostream>
#include <vector>
#include <string>

using namespace util_method;
TEST(param_pool, conf1) {

    std::vector<std::string> conf_names(1,"param_file.json");
    std::string conf_path = "./conf";
    auto param_pool = SingletonParamPool::Instance();
    EXPECT_TRUE(param_pool!=nullptr);
    param_pool->Init(conf_path, conf_names);
    LOG_INFO << param_pool->ParamToString();
    std::vector<std::string>* test_str_list = static_cast<std::vector<std::string>*>(param_pool->GetPredictParam("test_str_list"));
    std::stringstream ret;
    for (auto it = test_str_list->begin(); it != test_str_list->end(); ++ it) {
        if (it != test_str_list->begin()) {
            ret << ",";
        }
        ret << *it;
    }
    LOG_INFO << "GetPredictParam: test_str_list=" << ret.str();
    std::string* str_test = static_cast<std::string*>(param_pool->GetPredictParam("test_string"));
    LOG_INFO << "GetPredictParam: test_string=" << *str_test;
    double* param_test_num = static_cast<double*>(param_pool->GetPredictParam("test_num"));
    if (param_test_num==nullptr) {
        LOG_INFO << "param_test_num=nullptr";
    } else {
        LOG_INFO << "param_test_num=" << *param_test_num;
    }
    param_pool->GetPredictParam("test_no");
}
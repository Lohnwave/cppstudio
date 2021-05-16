/*
 * @Author: Lohnwave
 * @Date: 2021-03-27 13:10:24
 * @LastEditors: Lohnwave
 * @LastEditTime: 2021-05-16 16:33:56
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
    std::vector<std::string> str_list;
    param_pool->GetVecParam("test_str_list", str_list);
    for (uint32_t i = 0; i < str_list.size(); ++ i)
    {
        LOG_INFO << "test_str_list," << str_list[i];
    }
    std::vector<int> int_list;
    param_pool->GetVecParam("test_int_list", int_list);
    for (uint32_t i = 0; i < str_list.size(); ++ i)
    {
        LOG_INFO << "test_int_list," << int_list[i];
    }
}
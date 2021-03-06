//
//

#ifndef METHODTHIRDPARTY_DICT_PARAM_POOL_H
#define METHODTHIRDPARTY_DICT_PARAM_POOL_H


#include "thirdparty/jsoncpp-0.5.0/include/json/json.h"
#include "toft/base/singleton.h"
#include "toft/system/threading/thread_pool.h"
#include "thirdparty/muduo/base/ThreadPool.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <thread>

namespace methodthirdparty {
using CallBackMethod = std::function<void*(const std::string&)>;
using std::placeholders::_1;

struct PredictParametersConfig {
    // 请勿错误声明一个double k-v的v为整形, 如“test_1”:100, Json将会把它识别为int
    std::unordered_map<std::string, int> map_str_int; // "alpha":10000
    std::unordered_map<std::string, double> map_str_double; // "debug_ratio":0.5
    std::unordered_map<std::string, std::string> map_str_string; // "str": "subcats"
    
    std::unordered_map<std::string, std::vector<double>> map_str_double_list; //"double_list":"$double=0.33,0.0"
    std::unordered_map<std::string, std::vector<int>> map_str_int_list; // "int_list":"$int=50,0"
    std::unordered_map<std::string, std::vector<std::string>> map_str_str_list; // "str_list":"$string=cat,subcat"
    
    // std::unordered_map<std::string, Param_base*> map_str_param;
    std::unordered_map<std::string, CallBackMethod> param_get_method;
};
typedef std::shared_ptr<PredictParametersConfig> ParametersConfigPtr;
class ParamPool {
 public:
    ParamPool() {
        files_last_update_time_ = 0;
        param_buf_index_ = 0;
        param_buffers_[param_buf_index_] = std::make_shared<PredictParametersConfig>();
    }
    ~ParamPool() {}
    // SingletonRulePool::Instance()->Init(FLAGS_predictor_conf_path, confs_name);
    int Init(const std::string& path, std::vector<std::string>& conf_names);
    std::string ParamToString();
    bool AddParam(Json::Value& param_list, uint8_t param_buf_index);
    bool ParseStrValue(const std::string& param_name, const std::string& content, uint8_t param_buf_index);
    void* GetInt(const std::string& param_name);
    void* GetDouble(const std::string& param_name);
    void* GetString(const std::string& param);
    void* GetDoubleList(const std::string& param_name);
    void* GetIntList(const std::string& param_name);
    void* GetStringList(const std::string& param_name);
    // 使用时调用这个接口就可以
    void* GetPredictParam(const std::string& param_name);
    uint32_t ParamUpdate();
    bool Update();
    bool ParamIsRegistered(const std::string& param_name);
 private:
    std::vector<std::string> config_files_;
    long files_last_update_time_;
    std::unordered_set<std::string> all_param_names_;
    ParametersConfigPtr param_buffers_[2];
    uint8_t param_buf_index_;
};
typedef toft::Singleton<ParamPool> SingletonParamPool;
} // end namespace methodthirdparty
#endif // METHODTHIRDPARTY_DICT_PARAM_POOL_H

/*
 * @Author: Lohnwave
 * @Date: 2021-03-06 11:24:34
 * @LastEditors: Lohnwave
 * @LastEditTime: 2021-05-16 16:32:22
 * @Descripttion: 
 * @version: 
 */
#include "util_method/dict/param_pool.h"
#include "util_method/base/string_util.h"
#include "thirdparty/muduo/base/Logging.h"
#include "toft/crypto/hash/md5.h"
#include <fstream>
#include <sstream>

namespace util_method {
int ParamPool::Init(const std::string& path, std::vector<std::string>& conf_names) {
    // for ESC conf api get conf files from conf center
    std::chrono::milliseconds dura(10000); //10 s
    // std::this_thread::sleep_for(dura);
    Json::Reader read;
    Json::Value value;
    int error_num = 0, init_file_count = 0;
    for (uint32_t i = 0; i < conf_names.size(); ++ i) {
        std::string config_file = path + "/" + conf_names[i];
        config_files_.emplace_back(config_file);
        std::ifstream input(config_file);
        // Wait at most one minute to make sure the initialization is successful
        int wait_count = 0;
        while(!input.is_open() && wait_count < 30) {
            std::this_thread::sleep_for(dura/10);
            wait_count ++;
        }
        if (read.parse(input, value)) {
            AddParam(value, param_buf_index_);
        } else {
            input.close();
            error_num ++;
            continue;
        }
        LOG_INFO << "ParamPool: " << conf_names[i] << " init succeed!";
        input.close();
        init_file_count ++;
    }
    LOG_INFO << "ParamPool: finished init " << init_file_count << " files";
    return init_file_count;
}
// for test
std::string ParamPool::ParamToString() {
    std::stringstream ret;
    for (auto it = all_param_names_.begin(); it != all_param_names_.end(); ++ it) {
        if (it != all_param_names_.begin()) {
            ret << "\t";
        }
        ret << *it << "=";
        auto find_int = param_buffers_[param_buf_index_]->map_str_int.find(*it);
        if (find_int != param_buffers_[param_buf_index_]->map_str_int.end()) {
            ret << find_int->second;
            continue;
        }
        auto find_double = param_buffers_[param_buf_index_]->map_str_double.find(*it);
        if (find_double != param_buffers_[param_buf_index_]->map_str_double.end()) {
            ret << find_double->second;
            continue;
        }
        auto find_string = param_buffers_[param_buf_index_]->map_str_string.find(*it);
        if (find_string != param_buffers_[param_buf_index_]->map_str_string.end()) {
            ret << find_string->second;
            continue;
        }
        auto find_int_list = param_buffers_[param_buf_index_]->map_str_int_list.find(*it);
        if (find_int_list != param_buffers_[param_buf_index_]->map_str_int_list.end()) {
            for (auto it_v = find_int_list->second.begin(); it_v != find_int_list->second.end(); ++ it_v) {
                if (it_v != find_int_list->second.begin()) {
                    ret << ",";
                }
                ret << *it_v;
            }
            continue;
        }
        auto find_double_list = param_buffers_[param_buf_index_]->map_str_double_list.find(*it);
        if (find_double_list != param_buffers_[param_buf_index_]->map_str_double_list.end()) {
            for (auto it_v = find_double_list->second.begin(); it_v != find_double_list->second.end(); ++ it_v) {
                if (it_v != find_double_list->second.begin()) {
                    ret << ",";
                }
                ret << *it_v;
            }
            continue;
        }
        auto find_str_list = param_buffers_[param_buf_index_]->map_str_str_list.find(*it);
        if (find_str_list != param_buffers_[param_buf_index_]->map_str_str_list.end()) {
            for (auto it_v = find_str_list->second.begin(); it_v != find_str_list->second.end(); ++ it_v) {
                if (it_v != find_str_list->second.begin()) {
                    ret << ",";
                }
                ret << *it_v;
            }
            continue;
        }
    }
    return ret.str();
}
bool ParamPool::AddParam(Json::Value& param_list, uint8_t param_buf_index) {
    Json::Value::Members param_names = param_list.getMemberNames();
    for (Json::Value::Members::iterator it = param_names.begin(); it != param_names.end(); ++ it) {
        Json::ValueType param_type = param_list[*it].type();
        all_param_names_.insert(*it);
        switch (param_type) {
            case Json::intValue: {
                param_buffers_[param_buf_index]->map_str_int.insert({*it, param_list[*it].asInt()});
                param_buffers_[param_buf_index]->param_get_method.insert({*it, std::bind(&ParamPool::GetInt, this, _1)});
                break;
            }
            case Json::realValue: {
                param_buffers_[param_buf_index]->map_str_double.insert({*it, param_list[*it].asDouble()});
                param_buffers_[param_buf_index]->param_get_method.insert({*it, std::bind(&ParamPool::GetDouble, this, _1)});
                break;
            }
            case Json::stringValue: {
                param_buffers_[param_buf_index]->map_str_string.insert({*it, param_list[*it].asString()});
                param_buffers_[param_buf_index]->param_get_method.insert({*it, std::bind(&ParamPool::GetString, this, _1)});
                break;
            }
            case Json::arrayValue: {
                bool res = ParseArrayValue(*it, param_list[*it], param_buf_index);
                if (!res)
                {
                    return false;
                }
                break;
            }
            default : {
                LOG_ERROR << *it << "'s type not add in ParamPool!";
                break;
            }
        }
    }
    return true;
}
bool ParamPool::ParseArrayValue(
    const std::string& param_name, 
    Json::Value& array,
    uint8_t param_buf_index) {
    param_buffers_[param_buf_index]->array_size[param_name] = array.size();
    Json::ValueType array_type;
    Json::UInt first_idx = 0;
    if (array.size() > 0)
    {
        switch(array[first_idx].type())
        {
            case Json::intValue: 
            {
                param_buffers_[param_buf_index]->param_get_method.insert({param_name, std::bind(&ParamPool::GetIntList, this, _1)});
                array_type = Json::intValue;
                break;
            }
            case Json::realValue:
            {
                param_buffers_[param_buf_index]->param_get_method.insert({param_name, std::bind(&ParamPool::GetDoubleList, this, _1)});
                array_type = Json::realValue;
                break;
            }
            case Json::stringValue:
            {
                param_buffers_[param_buf_index]->param_get_method.insert({param_name, std::bind(&ParamPool::GetStringList, this, _1)});
                array_type = Json::stringValue;
                break;
            }
            default : 
            {
                LOG_ERROR << param_name << " array element not int/double/string...";
                return false;
            }
        }
    }
    else
    {
        return true; // 无法知道元素类型 则获取时适配各种类型
    }

    for (Json::Value::UInt i = 0; i < array.size() ; ++ i)
    {
        Json::Value& cur_element = array[i];
        switch(cur_element.type())
        {
            case Json::intValue : 
            {
                if (array_type != Json::intValue)
                {
                    LOG_ERROR << param_name << " array element type not equal..";
                    return false;
                }
                param_buffers_[param_buf_index]->map_str_int_list[param_name].emplace_back(cur_element.asInt());
                break;
            }
            case Json::realValue : 
            {
                if (array_type != Json::realValue)
                {
                    LOG_ERROR << param_name << " array element type not equal..";
                    return false;
                }
                param_buffers_[param_buf_index]->map_str_double_list[param_name].emplace_back(cur_element.asDouble());
                break;
            }
            case Json::stringValue : 
            {
                if (array_type != Json::stringValue)
                {
                    LOG_ERROR << param_name << " array element type not equal..";
                    return false;
                }
                param_buffers_[param_buf_index]->map_str_str_list[param_name].emplace_back(cur_element.asString());
                break;
            }
            default :
            {
                LOG_ERROR << param_name << " array element type not int/double/string..";
                return false;
            }
        }
    }
    return true;
}
void* ParamPool::GetInt(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->map_str_int.find(param_name);
    if (it == param_buffers_[param_buf_index_]->map_str_int.end()) {
        // LOG_ERROR << param_name << " not find!";
        static int default_int = 0;
        return &default_int;
    }
    return &(it->second);
}
void* ParamPool::GetDouble(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->map_str_double.find(param_name);
    if (it == param_buffers_[param_buf_index_]->map_str_double.end()) {
        // LOG_ERROR << param_name << " not find!";
        static double default_double = 0.0;
        return &default_double;
    }
    return &(it->second);
}
void* ParamPool::GetString(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->map_str_string.find(param_name);
    if (it == param_buffers_[param_buf_index_]->map_str_string.end()) {
        // LOG_ERROR << param_name << " not find!";
        static std::string default_string = "";
        return &default_string;
    }
    return &(it->second);
}
void* ParamPool::GetIntList(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->map_str_int_list.find(param_name);
    if (it == param_buffers_[param_buf_index_]->map_str_int_list.end()) {
        // LOG_ERROR << param_name << " not find!";
        static std::vector<int> default_int_list;
        return &default_int_list;
    }
    return &(it->second);
}
void* ParamPool::GetDoubleList(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->map_str_double_list.find(param_name);
    if (it == param_buffers_[param_buf_index_]->map_str_double_list.end()) {
        // LOG_ERROR << param_name << " not find!";
        static std::vector<double> default_double_list;
        return &default_double_list;
    }
    return &(it->second);
}
void* ParamPool::GetStringList(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->map_str_str_list.find(param_name);
    if (it == param_buffers_[param_buf_index_]->map_str_str_list.end()) {
        // OG_ERROR << param_name << " not find!";
        static std::vector<std::string> default_string_list;
        return &default_string_list;
    }
    return &(it->second);
}
void* ParamPool::GetPredictParam(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->param_get_method.find(param_name);
    if (it == param_buffers_[param_buf_index_]->param_get_method.end()) {
        // LOG_ERROR << param_name << " not find!";
        return nullptr;
    }
    auto func = it->second;
    return func(param_name);
}
uint32_t ParamPool::ParamUpdate() {
    uint32_t success_count = 0;
    bool md5_change_sign = false;
    for (uint32_t file_i = 0; file_i < config_files_.size(); ++ file_i) {
        if (Checkmd5(config_files_[file_i])) {
            md5_change_sign = true;
        }
    }
    for (uint32_t file_i = 0; file_i < config_files_.size(); ++ file_i) {
        struct stat statbuf;
        if(stat(config_files_[file_i].c_str(), &statbuf) == -1) {
            LOG_WARNING << config_files_[file_i] << " not exist";
            continue;
        }
        if (!(S_ISREG(statbuf.st_mode))) {
            LOG_ERROR << config_files_[file_i] << " is not valid";
            continue;
        }
        if (files_last_update_time_ < statbuf.st_mtime && md5_change_sign) {
            LOG_INFO << "updateing: " << config_files_[file_i] << " ...";
            if (file_i == 0) {
                param_buffers_[1-param_buf_index_] = std::make_shared<PredictParametersConfig>();
            }
            Json::Reader read;
            Json::Value value;
            std::ifstream input(config_files_[file_i]);
            if (!input.is_open()) {
                LOG_ERROR << config_files_[file_i] << " open error!...";
                continue;
            }
            if (read.parse(input, value)) {
                AddParam(value, 1-param_buf_index_);
            } else {
                LOG_WARNING << "ParamPool: ParamUpdate " << config_files_[file_i] << " update failed!";  
                continue;
            }
            input.close();
            success_count ++;
        }
        if (success_count == config_files_.size()) {
            files_last_update_time_ = statbuf.st_mtime;
        }
    }
    if (success_count == config_files_.size()) {
        param_buf_index_ = 1-param_buf_index_;
    }
    return success_count;
}
bool ParamPool::Update() {
    uint32_t success_count = 0;
    success_count = ParamUpdate();
    if (success_count > 0) {
        LOG_INFO << "ParamPool: ParamUpdate " << ParamToString();
    }
    LOG_INFO << "ParamPool: finished updated " << success_count << " files";
    return true;
}
bool ParamPool::Checkmd5(const std::string& config_file) {
    std::ifstream input(config_file);
    std::ostringstream file_data;
    char ch;
    while (file_data&&input.get(ch)) {
        file_data.put(ch);
    }
    if (file_data.str().length() == 0) {
        return false;
    }
    toft::MD5 md5;
    md5.Init();
    md5.Update(file_data.str());
    std::string file_content_md5 = std::move(md5.HexFinal());
    auto iter = config_file_md5_.find(config_file);
    if (iter == config_file_md5_.end()) {
        config_file_md5_.insert(std::make_pair(config_file, file_content_md5));
        LOG_INFO << "ParamPool: " << config_file << " md5=" << file_content_md5;
    } else {
        if (iter->second == file_content_md5) {
            LOG_INFO << "ParamPool: " << config_file << " unchanged";
            return false;
        } else {
            iter->second = file_content_md5;
            LOG_INFO << "ParamPool: " << config_file << " is changed md5=" << file_content_md5;
        }
    }
    return true;
}
bool ParamPool::ParamIsRegistered(const std::string& param_name) {
    auto it = param_buffers_[param_buf_index_]->param_get_method.find(param_name);
    if (it != param_buffers_[param_buf_index_]->param_get_method.end()) {
        return true;
    }
    return false;
}

} // end namespace util_method
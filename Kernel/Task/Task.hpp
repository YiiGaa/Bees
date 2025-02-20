/*
* 本文件内容不要人为修改，本文件由"python3 Christmas.py Input/MergeCode/LinkModule"生成
* 如果再次运行生成脚本，人为修改的代码将被删除
* 
* The contents of this file should not be modified. This file is generated by "python3 Christmas.py Input/MergeCode/LinkModule"
* If the generation script is run again, the artificially modified code will be deleted
*/

#ifndef   _TASK_Task_HPP_
#define   _TASK_Task_HPP_

#include <json/json.h>
#include <string>
#include <map>
#include <functional>
#include <array>

#include "Kernel/Task/Start/Start.hpp"
#include "Kernel/Task/SamplePush/SamplePush.hpp"
#include "Kernel/Task/SampleGet/SampleGet.hpp"

namespace Task{
    std::map<std::string, std::function<ERROR_CODE(Json::Value&)>> taskMap = {
        {"Start", Start::Start},
        {"SamplePush", SamplePush::Start},
        {"SampleGet", SampleGet::Start},
    };

    std::map<std::string, std::function<ERROR_CODE(Json::Value&)>> taskFunctionMap = {
        {"SamplePush::Do", SamplePush::Do},
        {"SampleGet::Do", SampleGet::Do},
    };
}
#endif
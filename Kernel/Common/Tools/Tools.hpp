#ifndef   _COMMON_Tools_HPP_
#define   _COMMON_Tools_HPP_ 

#define TOOLS_JsonClean(param) Common::Tools::JsonClean(param)
#define TOOLS_JsonRead(path, json) Common::Tools::JsonRead(path, json)
#define TOOLS_JsonValue(jsonPath, json, result) Common::Tools::JsonValue(jsonPath, json, result)

#define TOOLS_StringSplit(str, pattern) Common::Tools::StringSplit(str, pattern)
#define TOOLS_StringReplace(target, key, replacement) Common::Tools::StringReplace(target, key, replacement, 0)
#define TOOLS_StringReplaceOne(target, key, replacement) Common::Tools::StringReplace(target, key, replacement, 1)
#define TOOLS_StringStartWith(str, pattern) Common::Tools::StringStartWith(str, pattern)

#define TOOLS_ParamRead(key, defaultStr, moduleParam, passParam) Common::Tools::ParamRead(key, defaultStr, moduleParam, passParam)
#define TOOLS_ParamGet(key, defaultStr, passParam) Common::Tools::ParamGet(key, defaultStr, passParam)

#define TOOLS_EndCrashListen(param) Common::Tools::EndCrashListen(&param)
#define TOOLS_EndClean() Common::Tools::EndClean()
#define TOOLS_CallFunctionMark(...) Common::Tools::CallFunctionMark(__VA_ARGS__)
#define TOOLS_CallFunction(key, param) Common::Tools::CallFunction(key,param)
#define TOOLS_CallFunctionCheck(key) Common::Tools::CallFunctionCheck(key)
#define TOOLS_CallThreadMark(...) Common::Tools::CallThreadMark(__VA_ARGS__)
#define TOOLS_CallThread(key, param) Common::Tools::CallThread(key,param)
#define TOOLS_CallThreadCheck(key) Common::Tools::CallThreadCheck(key)

#define TOOLS_Lock(mutex, lockName) std::unique_lock lockName(mutex)
#define TOOLS_LockWrite(mutex, lockName) std::unique_lock lockName(mutex)
#define TOOLS_LockRead(mutex, lockName) std::shared_lock lockName(mutex)

#include <json/json.h>
#include <vector>
#include <string>
#include <map>
#include "Kernel/Module/Link.hpp"
#include <functional>

namespace Common{
    class Tools {
        public:
            static void JsonClean(Json::Value&);
            static int JsonRead(std::string &, Json::Value &);
            static bool JsonValue(std::string &, const Json::Value&, Json::Value&);

            static std::vector<std::string> StringSplit(const std::string &, const std::string &);
            static bool StringReplace(std::string &, const std::string &, const std::string &, int);
            static bool StringStartWith(std::string_view, const std::string &);

            static std::string ParamRead(std::string, const char*, const Json::Value&, const Json::Value&);
            static int64_t ParamRead(std::string, const int64_t, const Json::Value&, const Json::Value&);
            static int ParamRead(std::string, const int, const Json::Value&, const Json::Value&);
            static bool ParamRead(std::string, const bool, const Json::Value&, const Json::Value&);
            static double ParamRead(std::string, const double, const Json::Value&, const Json::Value&);
            static Json::Value ParamRead(std::string, Json::ValueType, const Json::Value&, const Json::Value&);
            static std::string ParamGet(std::string, std::string, const Json::Value&);

            static std::map<std::string, Module::Link*> *EndCrashListen_Map;
            static void EndCrashListen(std::map<std::string, Module::Link*>*);
            static void EndClean();

            static ERROR_CODE CallFunction(const std::string&, Json::Value&);
            static bool CallFunctionCheck(const std::string&);
            static void CallFunctionMark(const std::string&, std::function<ERROR_CODE(Json::Value&)>);
            static void CallFunctionMark(std::map<std::string, std::function<ERROR_CODE(Json::Value&)>>&);
            static ERROR_CODE CallThread(const std::string&, Json::Value&);
            static bool CallThreadCheck(const std::string&);
            static void CallThreadMark(const std::string&, std::function<ERROR_CODE(Json::Value&)>);
            static void CallThreadMark(std::map<std::string, std::function<ERROR_CODE(Json::Value&)>>&);

        private:
            static bool  JsonValue_Deep(std::vector<std::string>, const Json::Value&, Json::Value&);

            static void EndCrashListen_Crash(int);

            static std::map<std::string, std::function<ERROR_CODE(Json::Value&)>> CallFunction_Map;
            static std::map<std::string, std::function<ERROR_CODE(Json::Value&)>> CallThread_Map;
    };
}

#endif
#include "Kernel/Common/Tools/Tools.hpp"

//TIPS::Call function with string identity
//>>key::Function identity
//>>param::Data pool
//>>return::true/false
std::map<std::string, std::function<ERROR_CODE(Json::Value&)>> Common::Tools::CallFunction_Map;
ERROR_CODE Common::Tools::CallFunction(const std::string& key, Json::Value& param){
    ERROR_CODE ret = ERR_OK;
    if(CallFunction_Map.count(key)){
        LOGGER_LocateAdd("T-"+key);
        ret = CallFunction_Map[key](param);
        LOGGER_LocateDelete();
        return ret;
    }else{
        LOGGER_Error("TOOLS call function fail:"+key);
    }
    return ERR_OK;
}

//TIPS::Check whether the string identity with function
//>>key::Function identity
//>>return::true/false
bool Common::Tools::CallFunctionCheck(const std::string& key){
    if(CallFunction_Map.count(key)){
        return true;
    } else {
        return false;
    }
}

//TIPS::Mark function with identity
//------------
//>>key::Function identity
//      - you can use this identity to call function with TOOLS_CallFunction()
//>>startFun::Target function
//------------
//>>param::Function map
//------------
//>>return::void
void Common::Tools::CallFunctionMark(const std::string& key, std::function<ERROR_CODE(Json::Value&)> param){
    CallFunction_Map[key] = param;
    return;
}
void Common::Tools::CallFunctionMark(std::map<std::string, std::function<ERROR_CODE(Json::Value&)>>& param){
    CallFunction_Map = param;
    return;
}


//TIPS::Call thread function with string identity
//>>key::Thread Function identity
//>>param::Data pool
//>>return::true/false
std::map<std::string, std::function<ERROR_CODE(Json::Value&)>> Common::Tools::CallThread_Map;
ERROR_CODE Common::Tools::CallThread(const std::string& key, Json::Value& param){
    ERROR_CODE ret = ERR_OK;
    if(CallThread_Map.count(key)){
        LOGGER_LocateAdd(key);
        LOGGER_LocateAdd("T-Start");
        ret = CallThread_Map[key](param);
        LOGGER_LocateDelete();
        LOGGER_LocateDelete();
        return ret;
    }else{
        LOGGER_Error("TOOLS call thread fail:"+key);
    }
    return ret;
}

//TIPS::Check whether the string identity with thread function
//>>key::Thread function identity
//>>return::true/false
bool Common::Tools::CallThreadCheck(const std::string& key){
    if(CallThread_Map.count(key)){
        return true;
    } else {
        return false;
    }
}

//TIPS::Mark thread function with identity
//------------
//>>key::Function identity
//      - you can use this identity to call thread function with TOOLS_CallThread()
//>>startFun::Target function
//------------
//>>param::Function map
//------------
//>>return::void
void Common::Tools::CallThreadMark(const std::string& key, std::function<ERROR_CODE(Json::Value&)> startFun){
    CallThread_Map[key] = startFun;
    return;
}
void Common::Tools::CallThreadMark(std::map<std::string, std::function<ERROR_CODE(Json::Value&)>>& param){
    CallThread_Map = param;
    return;
}
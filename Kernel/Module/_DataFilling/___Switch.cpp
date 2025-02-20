#include "Kernel/Module/_DataFilling/_DataFilling.hpp"
#include <regex>

using namespace Module;

ERROR_CODE _DataFilling::Switch(Json::Value& target, Json::Value& setting, Json::Value& result, bool _isSwitchNullError){
    //STEP::Convert target to string
    ERROR_CODE ret = ERR_OK;
    std::string targetStr;
    if(target.type() == Json::ValueType::stringValue)
        targetStr = target.asString();
    else {
        Json::FastWriter fastWriter;
        fastWriter.omitEndingLineFeed();
        targetStr = fastWriter.write(target);
    }

    //STEP::Find matching value
    for(Json::ValueIterator it = setting.begin(); it != setting.end(); ++it){
        std::string key = it.key().asString();
        if(key == "")
            continue;
        if(key == targetStr){
            result = (*it);
            return ret;
        }
        if(TOOLS_StringStartWith(key, "reg##")){
            key.replace(0, 5, "");
            std::regex pattern(key);
            if(std::regex_search(targetStr, pattern)) {
                result = (*it);
                return ret;
            }
        }
    }

    //STEP::Try to use default value
    if(setting.isMember("")){
        result = setting[""];
        return ret;
    }

    //WHEN::Not matching anything
    result = target;
    if(_isSwitchNullError){
        LOGGER_Debug("Module-_DataFilling Switch get fail");
        return ERR_Module__DataFilling_Switch_Null;
    }
    return ret;
}
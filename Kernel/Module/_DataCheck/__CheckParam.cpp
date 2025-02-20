#include "Kernel/Module/_DataCheck/_DataCheck.hpp"
#include <regex>

using namespace Module;

int _DataCheck::CheckParam_Do(std::string setting, std::string value, Json::Value& passParam){
    bool isPass = false;

    //STEP::When allow anything
    if(setting == ""){
        return true;
    }

    //STEP::Whether regexp mode
    std::vector<std::string> settingArr = TOOLS_StringSplit(setting, "##");
    bool isRegexp = false;
    if(settingArr[0] == "reg"){
        isRegexp = true;
        setting.replace(0, 5, "");
        settingArr.erase(settingArr.begin());
    }

    //STEP::Get the value range is reversed or not
    bool isReversed = false;
    if(settingArr.size()>0 && settingArr[0] == "not"){
        isReversed = true;
        setting.replace(0, 5, "");
    }

    //WHEN::Regexp mode
    if(isRegexp){
        std::regex pattern(setting);
        if(std::regex_search(value, pattern)) {
            isPass = true;
        }
        return isReversed?!isPass:isPass;
    }

    //WHEN::Normal mode
    std::vector<std::string> rangeArr = TOOLS_StringSplit(setting, "/");
    for(const std::string& range:rangeArr) {
        if(range.find("get##") == 0){
            std::string tempRang = range;
            tempRang.replace(0, 5, "");
            std::vector<std::string> keyArr = TOOLS_StringSplit(tempRang, ">>");
            Json::Value* realRange = Traverse_FindJsonValue(keyArr, passParam);
            if(realRange->type()==Json::ValueType::stringValue &&
               realRange->asString() == value){
                isPass = true;
                break;
            } else if(realRange->type()==Json::ValueType::intValue &&
                      std::to_string(realRange->asInt()) == value){
                isPass = true;
                break;
            } else if(realRange->type()==Json::ValueType::booleanValue &&
                      std::to_string((int)realRange->asBool()) == value){
                isPass = true;
                break;
            }
        } else if(range == value){
            isPass = true;
            break;
        }
    }
    return isReversed?!isPass:isPass;
}

ERROR_CODE _DataCheck::CheckParam_TypeCheck(std::string& setting, std::string& value, Json::Value& target){
    //STEP::Get param type is int or string(default)
    bool isIllegal = false;
    std::vector<std::string> settingArr = TOOLS_StringSplit(setting, "##");
    if(settingArr[0] == "int"){
        if(target.type()==Json::ValueType::intValue || target.type()==Json::ValueType::booleanValue)
            value = std::to_string(target.asInt());
        else
            isIllegal = true;
        setting.replace(0, 5, "");
    } else if(settingArr[0] == "str"){
        if(target.type()==Json::ValueType::stringValue)
            value = target.asString();
        else
            isIllegal = true;
        setting.replace(0, 5, "");
    } else if(settingArr[0] == "double"){
        if(target.type()==Json::ValueType::intValue || target.type()==Json::ValueType::realValue) {
            double temp = target.asDouble();
            value = std::to_string(temp);
        }else
            isIllegal = true;
        setting.replace(0, 8, "");
    }  else if(settingArr[0] == "long"){
        if(target.type()==Json::ValueType::intValue || target.type()==Json::ValueType::booleanValue || target.type()==Json::ValueType::uintValue)
            value = target.asString();
        else
            isIllegal = true;
        setting.replace(0, 6, "");
    } else {
        if(target.type() == Json::ValueType::stringValue){
            value = target.asString();
        } else {
            Json::FastWriter fastWriter;
            fastWriter.omitEndingLineFeed();
            value = fastWriter.write(target);
        }
    }

    //STEP::Check param type
    if(isIllegal){
        return ERR_Module__DataCheck_ParamIllegal;
    }

    return ERR_OK;
}

ERROR_CODE _DataCheck::CheckParam(std::string setting, Json::Value& dataParam, Json::Value& resultParam, Json::Value& passParam){
    ERROR_CODE ret = ERR_OK;

    //STEP::Insert when allow anything
    if(setting == ""){
        resultParam = dataParam;
        return ret;
    }

    //STEP::Check type
    std::string value = "";
    ret = this->CheckParam_TypeCheck(setting, value, dataParam);
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Check the range of values and insert legal param
    bool isPass = CheckParam_Do(setting, value, passParam);
    if(!isPass){
        return ERR_Module__DataCheck_ParamIllegal;
    }else if(isPass){
        resultParam = dataParam;
    }

    return ret;
}
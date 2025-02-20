#include "Kernel/Common/Tools/Tools.hpp"

//std::string _keyStr = TOOLS_ParamRead("_keyStr", "-", moduleParam, passParam);
//int64_t _key64 = TOOLS_ParamRead("_key64", -1LL, moduleParam, passParam);
//int _keyInt = TOOLS_ParamRead("_keyInt", -1, moduleParam, passParam);
//bool _keyBool = TOOLS_ParamRead("_keyBool", false, moduleParam, passParam);
//double _keyDouble = TOOLS_ParamRead("_keyDouble", 0.0, moduleParam, passParam);
//Json::Value _keyValue = TOOLS_ParamRead("_keyValue", Json::objectValue, moduleParam, passParam);
//Json::Value _keyArray = TOOLS_ParamRead("_keyArray", Json::arrayValue, moduleParam, passParam);
//Json::Value _keyAny = TOOLS_ParamRead("_keyAny", Json::nullValue, moduleParam, passParam);

//TIPS::Get param according the key
//>>key::The key of the param
//>>defaultValue::The default value, the type of defaultValue also limit the type of target value
//              - It can be Json::nullValue, means not limiting the type
//              - It can be Json::objectValue, means limiting '{}' type
//              - It can be Json::arrayValue, means limiting '[]' type
//              - It can be string type, means limiting string type
//              - It can be int type, means limiting int type
//              - It can be int64 type, like '1LL', means limiting '{}' type
//              - It can be bool type, means limiting bool type
//              - It can be double type, means limiting double type
//>>moduleParam::Get the value corresponding to the 'key' in this object
//              - If the acquisition fails, the 'defaultValue' is returned;
//              - If the string starting with 'get##' is obtained, it will continue to search for value in 'passParam' as a 'new key';
//              - If the same value as the 'defaultValue' type is obtained, it will be used as the return value;
//>>passParam::Get the value corresponding to the 'new key'(get from moduleParam) in this object
//            - The 'new key' can be set as json path(use '>>' to locate), such as 'get##key_1>>2>>key_3';
//            - If the acquisition fails, the 'defaultValue' is returned;
//            - If the same value as the 'defaultValue' type is obtained, it will be used as the return value;
//>>return::Value
std::string Common::Tools::ParamRead(std::string key, const char* defaultStr, const Json::Value& moduleParam, const Json::Value& passParam){
    //WHEN::key not in moduleParam
    if(!moduleParam.isMember(key)) {
        return defaultStr;
    }
    Json::Value value = moduleParam[key];

    //WHEN::Get passParam by jsonPath (String start with get##)
    if(value.type()==Json::ValueType::stringValue){
        std::string valueStr = value.asString();

        //WHEN-IN::Try get from passParam
        if(TOOLS_StringStartWith(valueStr, "get##")){
            valueStr = valueStr.substr(5);
            Json::Value valueGet;
            if(TOOLS_JsonValue(valueStr, passParam, valueGet)){
                if(valueGet.type() == Json::ValueType::stringValue)
                    return valueGet.asString();
            }
            return defaultStr;
        }
    }

    //WHEN::Get from moduleParam
    if(value.type()==Json::ValueType::stringValue){
        return value.asString();
    }

    return defaultStr;
}

int64_t Common::Tools::ParamRead(std::string key, const int64_t defaultStr, const Json::Value& moduleParam, const Json::Value& passParam){
    //WHEN::key not in moduleParam
    if(!moduleParam.isMember(key)) {
        return defaultStr;
    }
    Json::Value value = moduleParam[key];

    //WHEN::Get passParam by jsonPath (String start with get##)
    if(value.type()==Json::ValueType::stringValue){
        std::string valueStr = value.asString();

        //WHEN-IN::Try get from passParam
        if(TOOLS_StringStartWith(valueStr, "get##")){
            valueStr = valueStr.substr(5);
            Json::Value valueGet;
            if(TOOLS_JsonValue(valueStr, passParam, valueGet)){
                if(valueGet.type()==Json::ValueType::intValue ||
                    valueGet.type()==Json::ValueType::booleanValue ||
                    valueGet.type()==Json::ValueType::uintValue
                )
                    return valueGet.asInt64();
            }
            return defaultStr;
        }
    }

    //WHEN::Get from moduleParam
    if(value.type()==Json::ValueType::intValue ||
       value.type()==Json::ValueType::booleanValue ||
       value.type()==Json::ValueType::uintValue
    ){
        return value.asInt64();
    }

    return defaultStr;
}

int Common::Tools::ParamRead(std::string key, const int defaultStr, const Json::Value& moduleParam, const Json::Value& passParam){
    //WHEN::key not in moduleParam
    if(!moduleParam.isMember(key)) {
        return defaultStr;
    }
    Json::Value value = moduleParam[key];

    //WHEN::Get passParam by jsonPath (String start with get##)
    if(value.type()==Json::ValueType::stringValue){
        std::string valueStr = value.asString();

        //WHEN-IN::Try get from passParam
        if(TOOLS_StringStartWith(valueStr, "get##")){
            valueStr = valueStr.substr(5);
            Json::Value valueGet;
            if(TOOLS_JsonValue(valueStr, passParam, valueGet)){
                if(valueGet.type()==Json::ValueType::intValue ||
                   valueGet.type()==Json::ValueType::booleanValue
                )
                    return valueGet.asInt();
            }
            return defaultStr;
        }
    }

    //WHEN::Get from moduleParam
    if(value.type()==Json::ValueType::intValue ||
       value.type()==Json::ValueType::booleanValue
    ){
        return value.asInt();
    }

    return defaultStr;
}

bool Common::Tools::ParamRead(std::string key, const bool defaultStr, const Json::Value& moduleParam, const Json::Value& passParam){
    //WHEN::key not in moduleParam
    if(!moduleParam.isMember(key)) {
        return defaultStr;
    }
    Json::Value value = moduleParam[key];

    //WHEN::Get passParam by jsonPath (String start with get##)
    if(value.type()==Json::ValueType::stringValue){
        std::string valueStr = value.asString();

        //WHEN-IN::Try get from passParam
        if(TOOLS_StringStartWith(valueStr, "get##")){
            valueStr = valueStr.substr(5);
            Json::Value valueGet;
            if(TOOLS_JsonValue(valueStr, passParam, valueGet)){
                if(valueGet.type()==Json::ValueType::intValue ||
                   valueGet.type()==Json::ValueType::booleanValue
                )
                    return valueGet.asBool();
            }
            return defaultStr;
        }
    }

    //WHEN::Get from moduleParam
    if(value.type()==Json::ValueType::intValue ||
       value.type()==Json::ValueType::booleanValue
    ){
        return value.asBool();
    }

    return defaultStr;
}

double Common::Tools::ParamRead(std::string key, const double defaultStr, const Json::Value& moduleParam, const Json::Value& passParam){
    //WHEN::key not in moduleParam
    if(!moduleParam.isMember(key)) {
        return defaultStr;
    }
    Json::Value value = moduleParam[key];

    //WHEN::Get passParam by jsonPath (String start with get##)
    if(value.type()==Json::ValueType::stringValue){
        std::string valueStr = value.asString();

        //WHEN-IN::Try get from passParam
        if(TOOLS_StringStartWith(valueStr, "get##")){
            valueStr = valueStr.substr(5);
            Json::Value valueGet;
            if(TOOLS_JsonValue(valueStr, passParam, valueGet)){
                if(valueGet.type()==Json::ValueType::intValue ||
                   valueGet.type()==Json::ValueType::realValue
                )
                    return valueGet.asDouble();
            }
            return defaultStr;
        }
    }

    //WHEN::Get from moduleParam
    if(value.type()==Json::ValueType::intValue ||
       value.type()==Json::ValueType::realValue
    ){
        return value.asDouble();
    }

    return defaultStr;
}

Json::Value Common::Tools::ParamRead(std::string key, Json::ValueType defaultStr, const Json::Value& moduleParam, const Json::Value& passParam){
    //WHEN::key not in moduleParam
    if(!moduleParam.isMember(key)) {
        return defaultStr;
    }
    Json::Value value = moduleParam[key];

    //WHEN::Get passParam by jsonPath (String start with get##)
    if(value.type()==Json::ValueType::stringValue){
        std::string valueStr = value.asString();

        //WHEN-IN::Try get from passParam
        if(TOOLS_StringStartWith(valueStr, "get##")){
            valueStr = valueStr.substr(5);
            Json::Value valueGet;
            if(TOOLS_JsonValue(valueStr, passParam, valueGet)){
                if(defaultStr==Json::nullValue || valueGet.type()==defaultStr)
                    return valueGet;
            }
            return defaultStr;
        }
    }

    //WHEN::Get from moduleParam
    if(defaultStr==Json::nullValue || value.type()==defaultStr) {
        return value;
    }

    return defaultStr;
}

//TIPS::Get param according the key, and It will be forcibly converted to string type
//>>key::The key of the param.The 'key' can be set as json path(starting with 'get##' and use '>>' to locate), such as 'get##key_1>>2>>key_3'
//>>defaultValue::The default value（must be string tpye）.
//>>param::Get the value corresponding to the 'key' in this object
//              - If the acquisition fails, the 'defaultValue' is returned;
//>>return::Value
std::string Common::Tools::ParamGet(std::string key, std::string defaultValue, const Json::Value& param){
    Json::Value result = defaultValue;

    //WHEN::Get param by key
    if(param.isMember(key))
        result = param[key];

    //WHEN::Get param by jsonPath (String start with get##)
    if(TOOLS_StringStartWith(key, "get##")){
        key = key.substr(5);
        TOOLS_JsonValue(key, param, result);
    }

    //WHEN::Convert to string
    if(result.type() == Json::ValueType::stringValue)
        return result.asString();
    else{
        Json::FastWriter fastWriter;
        fastWriter.omitEndingLineFeed();
        return fastWriter.write(result);
    }
}
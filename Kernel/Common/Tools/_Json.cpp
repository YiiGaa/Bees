#include "Kernel/Common/Tools/Tools.hpp"
#include <fstream>

//TIPS::Clean Json
//>>param::Target json
//>>return::void
void Common::Tools::JsonClean(Json::Value& param){
    if(!param.empty()){
        param.clear();
    }
    return;
}

//TIPS::Read file and convert into json value
//>>path::Target file path
//>>json::Result json value
//>>return::-1:false, 0:success
int Common::Tools::JsonRead(std::string &path, Json::Value &json){
    //STEP::Init
    Json::Reader jsonReader;
    std::ifstream jsonFile(path);

    //STEP::Read file and convert into json value
    //WHEN::Fail
    if(!jsonReader.parse(jsonFile, json)){
        jsonFile.close();
        return -1;
    }
    jsonFile.close();

    //WHEN::Success
    return 0;
}

//TIPS::Get json value
//>>path::Key path, can use ">>" to locate, such as "key 1>>key 2"
//>>data::Target json value
//>>result::Result get from target json, this value will be cloned, Modifying its value will not modify the original json
//>>return::true:get success/false:get false
bool Common::Tools::JsonValue(std::string &jsonPath, const Json::Value& data, Json::Value& result){
    //STEP::Split by ">>"
    std::vector<std::string> targetArr = TOOLS_StringSplit(jsonPath, ">>");

    //STEP::Get value
    return JsonValue_Deep(targetArr, data, result);
}
bool Common::Tools::JsonValue_Deep(std::vector<std::string> targetArr, const Json::Value& data, Json::Value& result){
    //WHEN::Return when find end
    if(targetArr.size() == 0 ||
       (targetArr.size() == 1 && targetArr[0]==""))
    {
        result = data;
        return true;
    }

    //STEP::Get find key
    std::string key = targetArr[0];
    targetArr.erase(targetArr.begin());

    //WHEN::Data is object
    if(data.type() == Json::ValueType::objectValue){
        if(data.isMember(key)) {
            return JsonValue_Deep(targetArr, data[key], result);
        }
        return false;
    }

    //WHEN::Data is array
    if(data.type() == Json::ValueType::arrayValue){
        //STEP-IN-IN::Change key to index
        int index = -1;
        try {
            index = std::stoi(key);
        } catch (const std::invalid_argument& e) {index = -1;} catch (const std::out_of_range& e) {index = -1;}

        //STEP-IN-IN::Check index is invalid or not
        if(index<0){
            return false;
        }

        //STEP-IN-IN::Find deeper when data has index
        if(data.size()>index){
            return JsonValue_Deep(targetArr, data[index], result);
        }
    }

    return false;
}

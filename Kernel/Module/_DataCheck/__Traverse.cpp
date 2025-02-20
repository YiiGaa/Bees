#include "Kernel/Module/_DataCheck/_DataCheck.hpp"
#include <regex>

using namespace Module;

int _DataCheck::Traverse_ListMaxLength(std::string& param){
    int maxLength = -1;
    //STEP::Replace number##
    std::smatch matches;
    std::regex regexMaxLength(R"(^\d+##)");
    if (std::regex_search(param, matches, regexMaxLength)) {
        std::string matchStr = matches[0].str();
        TOOLS_StringReplaceOne(param, matchStr, "");
        TOOLS_StringReplace(matchStr, "##", "");
        maxLength = std::stoi(matchStr);
    }
    return maxLength;
}

Json::Value* _DataCheck::Traverse_FindJsonValue(std::vector<std::string> targetArr, Json::Value& data){
    //WHEN::Return when find end
    if(targetArr.size() == 0 ||
       (targetArr.size() == 1 && targetArr[0]=="")){
        return &data;
    }

    //STEP::Get find key
    std::string key = targetArr[0];
    targetArr.erase(targetArr.begin());

    //WHEN::Find deeper when data is object and data has this key
    if(data.type() == Json::ValueType::objectValue && data.isMember(key)){
        return this->Traverse_FindJsonValue(targetArr, data[key]);
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
            return NULL;
        }

        //STEP-IN-IN::Find deeper when data has index
        if(data.size()>index){
            return this->Traverse_FindJsonValue(targetArr, data[index]);
        }
    }
    return NULL;
}

Json::Value* _DataCheck::Traverse_InitJsonPath(std::vector<std::string> targetArr, Json::Value& data){
    //WHEN::Return when init end
    if(targetArr.size() == 0){
        return &data;
    }

    //STEP::Get find key
    std::string key = targetArr[0];
    targetArr.erase(targetArr.begin());

    //STEP::Try get index
    int index = -1;
    try {
        index = std::stoi(key);
    } catch (const std::invalid_argument& e) {index = -1;} catch (const std::out_of_range& e) {index = -1;}

    //WHEN::Find deeper when data is object and data has this key
    if(data.type() == Json::ValueType::objectValue && data.isMember(key)){
        return Traverse_InitJsonPath(targetArr, data[key]);
    }

    //WHEN::Find deeper when data is array and data has this index
    if(data.type() == Json::ValueType::arrayValue && index>-1){
        while(data.size()<=index){
            data.append(Json::objectValue);
        }
        return Traverse_InitJsonPath(targetArr, data[index]);
    }

    //STEP::Cover init when is not found
    data[key] = Json::objectValue;

    //WHEN::Find deeper after init when data is object and data has this key
    if(data.type() == Json::ValueType::objectValue){
        data[key] = Json::objectValue;
        return Traverse_InitJsonPath(targetArr, data[key]);
    }

    return NULL;
}

ERROR_CODE _DataCheck::Traverse(Json::Value& templateParam, Json::Value& dataParam, Json::Value& resultParam, Json::Value& passParam){
    ERROR_CODE ret = ERR_OK;

    Json::ValueType templateType = templateParam.type();

    //WHEN::Direct judge
    if(templateType!=Json::ValueType::stringValue &&
       templateType!=Json::ValueType::objectValue &&
       templateType!=Json::ValueType::arrayValue){
        if(templateParam == dataParam){
            resultParam = dataParam;
            return ret;
        } else {
            return ERR_Module__DataCheck_ParamIllegal;
        }
    }

    //WHEN::Check range and insert data
    if(templateType==Json::ValueType::stringValue){
        ret = this->CheckParam(templateParam.asString(), dataParam, resultParam, passParam);
        return ret;
    }

    //STEP-IN::Check param type
    if(dataParam.type()!=templateType){
        return ERR_Module__DataCheck_ParamTypeIllegal;
    }

    //WHEN::Object value
    if(templateType == Json::ValueType::objectValue){
        //STEP-IN::Allow anything when template is {}
        if(templateParam.empty()){
            resultParam = dataParam;
            return ret;
        }

        //STEP-IN::Modify resultParam type
        if(resultParam.type()!=Json::ValueType::objectValue){
            resultParam = Json::objectValue;
        }

        //STEP-IN::Traverse object
        for(Json::ValueIterator it = templateParam.begin(); it != templateParam.end(); ++it){
            std::string key = it.key().asString();
            Json::ValueType type = (*it).type();

            //STEP-IN::Get key is option or necessary(default)
            bool isNecessary = true;
            if(TOOLS_StringStartWith(key, "opt##")){
                key.replace(0, 5, "");
                isNecessary = false;
            } else if(TOOLS_StringStartWith(key, "nec##")){
                key.replace(0, 5, "");
                isNecessary = true;
            }

            //STEP-IN::Get array size limit
            int limitSize = -1;
            if(type == Json::ValueType::arrayValue){
                limitSize = this->Traverse_ListMaxLength(key);
            }

            //STEP-IN::Get real data param
            std::vector<std::string> keyArr = TOOLS_StringSplit(key, ">>");
            Json::Value* realDataParam = Traverse_FindJsonValue(keyArr, dataParam);

            //STEP-IN::Skip lack option key
            if(!isNecessary && realDataParam==NULL)
                continue;

            //STEP-IN::Check whether the data contains necessary key
            if(isNecessary && realDataParam==NULL){
                LOGGER_Debug("Module-_DataCheck lack param, key:"+key);
                return ERR_Module__DataCheck_ParamLack;
            }

            //STEP-IN::Init resultParam
            Json::Value* realResultParam = Traverse_FindJsonValue(keyArr, resultParam);
            if(realResultParam==NULL){
                realResultParam = Traverse_InitJsonPath(keyArr, resultParam);
                *realResultParam = "";
            }

            //STEP-IN::Traverse call
            ret = this->Traverse((*it), *realDataParam, *realResultParam, passParam);
            if(ERRORCODE_IsError(ret)){
                LOGGER_Debug("Module-_DataCheck param illegal, key:"+key);
                return ret;
            }

            //STEP-IN::Check array size limit
            if(limitSize>-1 && realDataParam->type() == Json::ValueType::arrayValue){
                if(realDataParam->size()>limitSize){
                    LOGGER_Debug("Module-_DataCheck param array size over limit, key:"+key);
                    return ERR_Module__DataCheck_ArrayLimit;
                }
            }
        }
        return ret;
    }

    //WHEN::Array value
    if(templateType == Json::ValueType::arrayValue){
        //STEP-IN::Allow anything when template is []
        if(templateParam.empty()){
            resultParam = dataParam;
            return ret;
        }

        //STEP-IN::Modify resultParam type
        if(resultParam.type()!=Json::ValueType::arrayValue){
            resultParam = Json::arrayValue;
        }

        for(Json::ValueIterator it = dataParam.begin(); it != dataParam.end(); ++it){
            int index = it.index();

            //STEP-IN::Find match template and traverse call
            ERROR_CODE innerRet = ERR_Module__DataCheck_ParamIllegal;
            for(Json::ValueIterator itTemplate = templateParam.begin(); itTemplate != templateParam.end(); ++itTemplate){
                if((*itTemplate).type()!=Json::ValueType::stringValue &&
                   (*itTemplate).type() != (*it).type()){
                    continue;
                }

                //STEP-IN-IN::Append init element
                if(index>=resultParam.size()){
                    resultParam.append("");
                }

                //STEP-IN-IN::Traverse call
                innerRet = this->Traverse((*itTemplate), dataParam[index], resultParam[index], passParam);
                if(!ERRORCODE_IsError(innerRet)){
                    break;
                }
            }

            if(ERRORCODE_IsError(innerRet)){
                LOGGER_Debug("Module-_DataCheck param illegal, index:"+std::to_string(index));
                return ERR_Module__DataCheck_ParamIllegal;
            }
        }
        return ret;
    }

    return ret;
}
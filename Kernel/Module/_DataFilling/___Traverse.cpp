#include "Kernel/Module/_DataFilling/_DataFilling.hpp"
#include <regex>

using namespace Module;

bool _DataFilling::Traverse_CustomKey(std::string& param, Json::Value& passParam){
    int isReplace = true;
    //STEP::Replace custom key
    std::smatch matches;
    std::regex regexFill("@(.+?)@");
    while(std::regex_search(param, matches, regexFill)) {
        std::string matchStr = matches[0].str();
        TOOLS_StringReplace(matchStr, "@", "");
        matchStr = "+"+matchStr;
        Json::Value result = this->Filling(matchStr, passParam);
        TOOLS_StringReplaceOne(matchStr, "+", "");
        if(result.type() == Json::ValueType::stringValue){
            std::string replace = result.asString();
            TOOLS_StringReplaceOne(replace, "+", "");
            TOOLS_StringReplace(param, "@"+matchStr+"@", replace);
            isReplace = true;
        } else {
            TOOLS_StringReplace(param, "@"+matchStr+"@", "");
        }
    }
    return isReplace;
}

Json::Value* _DataFilling::Traverse_FindJsonValue(std::vector<std::string> targetArr, Json::Value& data, bool isDelete){
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
        if(targetArr.size() == 0 && isDelete){
            data.removeMember(key);
            return &data;
        }
        return this->Traverse_FindJsonValue(targetArr, data[key], isDelete);
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
            if(targetArr.size() == 0 && isDelete){
                data.removeIndex(index, nullptr);
                return &data;
            }
            return this->Traverse_FindJsonValue(targetArr, data[index], isDelete);
        }
    }
    return NULL;
}

Json::Value* _DataFilling::Traverse_InitJsonPath(std::vector<std::string> targetArr, Json::Value& data){
    //WHEN::Return when init end
    if(targetArr.size() == 0){
        return &data;
    }

    //STEP::Get find key
    std::string key = targetArr[0];
    targetArr.erase(targetArr.begin());

    //STEP::Try to get index
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

ERROR_CODE _DataFilling::Traverse(Json::Value& templateParam, Json::Value& resultParam, Json::Value& passParam, bool _isSwitchNullError){
    ERROR_CODE ret = ERR_OK;
    Json::ValueType templateType = templateParam.type();

    //WHEN::Direct insert
    if(templateType!=Json::ValueType::stringValue &&
       templateType!=Json::ValueType::objectValue &&
       templateType!=Json::ValueType::arrayValue){
        resultParam = templateParam;
        return ret;
    }

    //WHEN::Filling data
    if(templateType==Json::ValueType::stringValue){
        std::string target = templateParam.asString();
        Json::Value result = this->Filling(target, passParam);
        resultParam = result;
        return ret;
    }

    //WHEN::Object value
    if(templateType == Json::ValueType::objectValue){
        //STEP-IN::Cover object when resultParam type not match
        if(resultParam.type()!=Json::ValueType::objectValue){
            resultParam = Json::objectValue;
        }

        for(Json::ValueIterator it = templateParam.begin(); it != templateParam.end(); ++it){
            std::string key = it.key().asString();
            Json::ValueType type = (*it).type();

            //STEP-IN::Replace custom key, like "key_@get##id@"
            this->Traverse_CustomKey(key, passParam);

            //STEP-IN::Get key is cover(default/nec) or not cover(opt)
            bool isCover = true;
            if(TOOLS_StringStartWith(key, "opt##")){
                key.replace(0, 5, "");
                isCover = false;
            } else if(TOOLS_StringStartWith(key, "nec##")){
                key.replace(0, 5, "");
                isCover = true;
            }

            //STEP-IN::Get key is push(Array)
            bool isPush = false;
            if(type == Json::ValueType::arrayValue && TOOLS_StringStartWith(key, "push##")){
                key.replace(0, 6, "");
                isPush = true;
            }

            //STEP-IN::Get key is switch(Object)
            bool isSwitch = false;
            if(type == Json::ValueType::objectValue && TOOLS_StringStartWith(key, "switch##")){
                key.replace(0, 8, "");
                isSwitch = true;
            }

            //STEP-IN::Get real param(">>" deep get), and skip not cover key
            std::vector<std::string> keyArr = TOOLS_StringSplit(key, ">>");
            Json::Value* realResultParam = this->Traverse_FindJsonValue(keyArr, resultParam, false);
            if(!isCover && realResultParam!=NULL)
                continue;
            if(isSwitch && realResultParam==NULL) {
                LOGGER_Info("Module-_DataFilling Traverse skip switch key:"+key);
                continue;
            }

            //STEP-IN::Delete key when value is null
            if(it->isNull()){
                if(realResultParam!=NULL) {
                    LOGGER_Debug("Module-_DataFilling Traverse delete key:"+key);
                    Traverse_FindJsonValue(keyArr, resultParam, true);
                }
                continue;
            }

            //WHEN-IN::Init value (not in target)
            if(realResultParam == NULL){
                realResultParam = this->Traverse_InitJsonPath(keyArr, resultParam);
            }
            if(realResultParam->type() != Json::ValueType::arrayValue)
                isPush = false;

            //WHEN-IN::Deal other
            if(isPush) {
                Json::Value pieceResult = Json::arrayValue;
                ret = this->Traverse((*it), pieceResult, passParam, _isSwitchNullError);
                if(ERRORCODE_IsError(ret)){
                    return ret;
                }
                for (const auto& item : pieceResult) {
                    realResultParam->append(item);
                }
            } else {
                LOGGER_Debug("Module-_DataFilling Traverse deal key:"+key+", isSwitch:"+std::to_string(isSwitch));
                if(isSwitch) {
                    Json::Value result;
                    ret = this->Switch(*realResultParam, (*it), result, _isSwitchNullError);
                    *realResultParam = result;
                }else
                    ret = this->Traverse((*it), *realResultParam, passParam, _isSwitchNullError);
                if(ERRORCODE_IsError(ret)){
                    return ret;
                }
            }
        }
        return ret;
    }

    //WHEN::Array value
    if(templateType == Json::ValueType::arrayValue){
        //STEP-IN::Cover object when resultParam type not match
        if(resultParam.type()!=Json::ValueType::arrayValue){
            resultParam = Json::arrayValue;
        }

        //WHEN-IN::Empty array when templateParam empty
        if(templateParam.size()==0){
            resultParam = Json::arrayValue;
            return ret;
        }

        //STEP-IN::Create
        for(Json::ValueIterator it = templateParam.begin(); it != templateParam.end(); ++it){
            int index = it.index();
            if(index<resultParam.size()){
                ret = this->Traverse((*it), resultParam[index], passParam, _isSwitchNullError);
                if(ERRORCODE_IsError(ret)){
                    return ret;
                }
            }else{
                Json::Value pieceResult = Json::objectValue;
                ret = this->Traverse((*it), pieceResult, passParam, _isSwitchNullError);
                if(ERRORCODE_IsError(ret)){
                    return ret;
                }
                resultParam.append(pieceResult);
            }
        }
        return ret;
    }

    return ret;
}
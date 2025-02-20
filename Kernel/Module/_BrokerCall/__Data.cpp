#include "Kernel/Module/_BrokerCall/_BrokerCall.hpp"

using namespace Module;

ERROR_CODE _BrokerCall::DataCreate(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerCall DataCreate _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_LockWrite(this->innerParamMutex, lock);

    //WHEN::Already exist
    if(this->innerParamMap.count(_identity)){
        innerParam = this->innerParamMap[_identity];
        innerParam->useCount++;
        return ret;
    }

    //STEP::Create data
    innerParam_t *innerParamPtr = new innerParam_t();
    this->innerParamMap[_identity] = innerParamPtr;
    innerParam = innerParamPtr;
    innerParam->useCount++;

    return ret;
}

ERROR_CODE _BrokerCall::DataGet(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerCall DataGet _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_LockRead(this->innerParamMutex, lock);

    //WHEN::Not exist, error
    if(!this->innerParamMap.count(_identity)){
        LOGGER_Error("Module-_BrokerCall DataGet identity not exist, _identity"+_identity);
        return ERR_Module__BrokerCall_DataGet_Fail;
    }

    //STEP::Get innerParam
    innerParam = this->innerParamMap[_identity];
    innerParam->useCount++;

    return ret;
}

ERROR_CODE _BrokerCall::DataDelete(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //WHEN::Not data owner
    if(innerParam==NULL){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerCall DataDelete _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_LockWrite(this->innerParamMutex, lock);

    //STEP::Check use count
    innerParam->useCount--;
    if(innerParam->useCount>0)
        return ret;

    //WHEN::Not exist, error
    if(!this->innerParamMap.count(_identity)){
        LOGGER_Error("Module-_BrokerCall DataDelete identity not exist, _identity"+_identity);
        return ERR_Module__BrokerCall_DataDelete_NotExist;
    }

    //STEP::Delete innerParam
    delete innerParam;
    innerParam = NULL;
    this->innerParamMap.erase(_identity);

    return ret;
}
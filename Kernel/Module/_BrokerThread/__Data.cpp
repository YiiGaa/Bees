#include "Kernel/Module/_BrokerThread/_BrokerThread.hpp"

using namespace Module;

ERROR_CODE _BrokerThread::DataCreate(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerThread DataCreate _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_Lock(innerParamMutex, lock);

    //WHEN::Already exist
    if(innerParamMap.count(_identity)){
        LOGGER_Error("Module-_BrokerThread DataCreate already exist _identity:" + _identity);
        return ERR_Module__BrokerThread_DataCreate_Fail;
    }

    //STEP::Create data
    innerParam_t *innerParamPtr = new innerParam_t();
    innerParamMap[_identity] = innerParamPtr;
    innerParam = innerParamPtr;
    innerParam->useCount++;

    return ret;
}

ERROR_CODE _BrokerThread::DataGet(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerThread DataGet _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_Lock(innerParamMutex, lock);

    //WHEN::Not exist, error
    if(!innerParamMap.count(_identity)){
        LOGGER_Error("Module-_BrokerThread DataGet identity not exist, _identity"+_identity);
        return ERR_Module__BrokerThread_DataGet_Fail;
    }

    //STEP::Get innerParam
    innerParam = innerParamMap[_identity];
    innerParam->useCount++;

    return ret;
}

ERROR_CODE _BrokerThread::DataDelete(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //WHEN::Not data owner
    if(innerParam==NULL){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerThread DataDelete _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_Lock(innerParamMutex, lock);

    //STEP::Check use count
    innerParam->useCount--;
    if(innerParam->useCount>0)
        return ret;

    //WHEN::Not exist, error
    if(!innerParamMap.count(_identity)){
        LOGGER_Error("Module-_BrokerThread DataDelete identity not exist, _identity"+_identity);
        return ERR_Module__BrokerThread_DataDelete_NotExist;
    }

    //STEP::Delete innerParam
    delete innerParam;
    innerParam = NULL;
    innerParamMap.erase(_identity);

    //STEP::Notify WailAll()
    innerParamCondWait.notify_all();

    return ret;
}

ERROR_CODE _BrokerThread::DataDeleteThread(innerParam_t*& innerParam){
    ERROR_CODE ret = ERR_OK;
    std::string _identity = innerParam->_identity;

    //WHEN::Not data owner
    if(innerParam==NULL){
        return ret;
    }

    //STEP::Lock thread
    TOOLS_Lock(innerParamMutex, lock);

    //STEP::Check use count
    innerParam->useCount--;
    if(innerParam->useCount>0)
        return ret;

    //WHEN::Not exist, error
    if(!innerParamMap.count(_identity)){
        LOGGER_Error("Module-_BrokerThread DataDeleteThread identity not exist, _identity"+_identity);
        return ERR_Module__BrokerThread_DataDelete_NotExist;
    }

    //STEP::Delete innerParam
    delete innerParam;
    innerParam = NULL;
    innerParamMap.erase(_identity);

    //STEP::Notify WailAll()
    innerParamCondWait.notify_all();

    return ret;
}
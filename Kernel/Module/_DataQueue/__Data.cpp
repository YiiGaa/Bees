#include "Kernel/Module/_DataQueue/_DataQueue.hpp"

using namespace Module;

ERROR_CODE _DataQueue::DataCreate(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    int _queueMaxLen = TOOLS_ParamRead("_queueMaxLen", -1, moduleParam, passParam);
    LOGGER_Debug("Module-_DataQueue DataCreate _identity:" + _identity);
    LOGGER_Debug("Module-_DataQueue DataCreate _queueMaxLen:" + std::to_string(_queueMaxLen));

    //STEP::Lock thread
    TOOLS_LockWrite(this->innerParamMutex, lock);

    //WHEN::Already exist
    if(this->innerParamMap.count(_identity)){
        LOGGER_Error("Module-_DataQueue DataCreate identity exist, _identity"+_identity);
        return ERR_Module__DataQueue_DataCreate_Exist;
    }

    //STEP::Create data
    innerParam_t *innerParamPtr = new innerParam_t();
    this->innerParamMap[_identity] = innerParamPtr;
    innerParam = innerParamPtr;
    innerParam->useCount++;
    innerParam->_queueMaxLen = _queueMaxLen;

    return ret;
}

ERROR_CODE _DataQueue::DataGet(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_DataQueue DataGet _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_LockRead(this->innerParamMutex, lock);

    //WHEN::Not exist, error
    if(!this->innerParamMap.count(_identity)){
        LOGGER_Error("Module-_DataQueue DataGet identity not exist, _identity"+_identity);
        return ERR_Module__DataQueue_DataGet_Fail;
    }

    //STEP::Get innerParam
    innerParam = this->innerParamMap[_identity];
    innerParam->useCount++;

    return ret;
}

ERROR_CODE _DataQueue::DataDelete(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //WHEN::Not data owner
    if(innerParam==NULL){
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_DataQueue DataDelete _identity:" + _identity);

    //STEP::Lock thread
    TOOLS_LockWrite(this->innerParamMutex, lock);

    //STEP::Check use count
    innerParam->useCount--;
    if(innerParam->useCount>0)
        return ret;

    //WHEN::Not exist, error
    if(!this->innerParamMap.count(_identity)){
        LOGGER_Error("Module-_DataQueue DataDelete identity not exist, _identity"+_identity);
        return ERR_Module__DataQueue_DataDelete_NotExist;
    }

    //STEP::Clean queue
    for (size_t i = 0; i < innerParam->queue.size(); i++){
        delete innerParam->queue[i];
        innerParam->queue[i] = NULL;
    }

    //STEP::Delete innerParam
    delete innerParam;
    innerParam = NULL;
    this->innerParamMap.erase(_identity);

    return ret;
}
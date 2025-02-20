#include "Kernel/Module/_BrokerThread/_BrokerThread.hpp"

using namespace Module;

ERROR_CODE _BrokerThread::Wait(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Lock
    TOOLS_Lock(innerParam->mutex, lock);
    LOGGER_Info("_BrokerThread Wait identity:"+innerParam->_identity);
    if(!innerParam->isEnd){
        innerParam->condWait.wait(lock);
    }
    LOGGER_Info("_BrokerThread Wait end identity:"+innerParam->_identity);

    return ret;
}

ERROR_CODE _BrokerThread::WaitAll(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Lock
    TOOLS_LockWrite(innerParamMutex, lock);

    //STEP::Wait end
    while(innerParamMap.size() > 0){
        innerParamCondWait.wait(lock);
    }

    return ret;
}
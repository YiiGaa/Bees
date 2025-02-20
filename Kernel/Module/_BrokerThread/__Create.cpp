#include "Kernel/Module/_BrokerThread/_BrokerThread.hpp"

using namespace Module;

void _BrokerThread::Create_Do(std::string identity){
    //STEP::Call task start
    innerParam_t* innerParam = innerParamMap[identity];
    LOGGER_Info("_BrokerThread Create_Do thread, _identity:"+innerParam->_identity);
    ERROR_CODE ret = TOOLS_CallThread(innerParam->_call, innerParam->_param);
    LOGGER_Info("_BrokerThread Create_Do return, _identity:"+innerParam->_identity+", errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret));

    //STEP::Mark end
    TOOLS_Lock(innerParam->mutex, lock);
    innerParam->isEnd = true;
    innerParam->condWait.notify_all();
    lock.unlock();

    //STEP::Clean data
    DataDeleteThread(innerParam);
    return;
}

ERROR_CODE _BrokerThread::Create(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    Json::Value _param = moduleParam.isMember("_param")?TOOLS_ParamRead("_param", Json::objectValue, moduleParam, passParam):passParam;
    std::string _call = TOOLS_ParamRead("_call", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerThread Create _identity:" + _identity);
    LOGGER_Debug("Module-_BrokerThread Create _param:" + _param.toStyledString());
    LOGGER_Debug("Module-_BrokerThread Create _call:" + _call);

    //STEP::Check thread
    if(!TOOLS_CallThreadCheck(_call)){
        LOGGER_Error("_BrokerThread Create thread lack:"+_call);
        return ERR_Module__BrokerThread_Create_Lack;
    }

    //STEP::Create thread
    innerParam->_param = Json::objectValue;
    innerParam->_param = _param;
    innerParam->_call = _call;
    innerParam->_identity = _identity;
    LOGGER_Info("_BrokerThread Create thread:"+_call+", _identity:"+_identity);
    std::thread threadHandler(Create_Do, _identity);
    threadHandler.detach();

    return ret;
}

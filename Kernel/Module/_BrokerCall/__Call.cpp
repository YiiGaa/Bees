#include "Kernel/Module/_BrokerCall/_BrokerCall.hpp"
#include <unistd.h>

using namespace Module;

ERROR_CODE _BrokerCall::Call(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _call = TOOLS_ParamRead("_call", "", moduleParam, passParam);
    int _loopCount = TOOLS_ParamRead("_loopCount", 1, moduleParam, passParam);
    int64_t _sleepTime = TOOLS_ParamRead("_sleepTime", 1000000LL, moduleParam, passParam);
    bool _isErrorStop = TOOLS_ParamRead("_isErrorStop", false, moduleParam, passParam);
    bool _isAdoptFunRet = TOOLS_ParamRead("_isAdoptFunRet", true, moduleParam, passParam);
    Json::Value _param = moduleParam.isMember("_param")?TOOLS_ParamRead("_param", Json::objectValue, moduleParam, passParam):passParam;
    std::string _errorKey = TOOLS_ParamRead("_errorKey", "", moduleParam, passParam);
    std::string _messageKey = TOOLS_ParamRead("_messageKey", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerCall Call _call:" + _call);
    LOGGER_Debug("Module-_BrokerCall Call _loopCount:" + std::to_string(_loopCount));
    LOGGER_Debug("Module-_BrokerCall Call _sleepTime:" + std::to_string(_sleepTime));
    LOGGER_Debug("Module-_BrokerCall Call _isErrorStop:" + std::to_string(_isErrorStop));
    LOGGER_Debug("Module-_BrokerCall Call _isAdoptFunRet:" + std::to_string(_isAdoptFunRet));
    LOGGER_Debug("Module-_BrokerCall Call _param:" + _param.toStyledString());
    LOGGER_Debug("Module-_BrokerCall Call _errorKey:" + _errorKey);
    LOGGER_Debug("Module-_BrokerCall Call _messageKey:" + _messageKey);

    //STEP::Check Function
    if(!TOOLS_CallFunctionCheck(_call)){
        LOGGER_Error("_BrokerCall Call function lack:"+_call);
        return ERR_Module__BrokerCall_Call_Lack;
    }

    //STEP::Call function
    ERROR_CODE retFun = ERR_OK;
    while(true){
        //STEP-IN::Call function
        if(_loopCount>0) _loopCount--;
        LOGGER_Info("_BrokerCall Call call:"+_call+", remain:"+std::to_string(_loopCount));
        retFun = TOOLS_CallFunction(_call, _param);

        //STEP-IN::Check loop
        if(_loopCount == 0){
            break;
        }

        //STEP-IN::Check result
        if(_isErrorStop && ERRORCODE_IsError(retFun)){
            LOGGER_Debug("Module-_BrokerCall Call error interrupt, call:"+_call+", errorCode:"+ERRORCODE_GetCode(retFun)+", message:"+ERRORCODE_GetMessage(retFun));
            break;
        }

        //STEP-IN::Check whether stop
        {
            if(innerParam->isCallEnd.load()) {
                LOGGER_Debug("Module-_BrokerCall Call end interrupt, call:"+_call);
                break;
            }
            if(this->isCrash) {
                LOGGER_Debug("Module-_BrokerCall Call crash interrupt, call:"+_call);
                break;
            }
        }

        //STEP-IN::Sleep, and Check whether stop again
        if(_sleepTime>0){
            usleep(_sleepTime);

            if(innerParam->isCallEnd.load()) {
                LOGGER_Debug("Module-_BrokerCall Call end interrupt, call:"+_call);
                break;
            }
            if(this->isCrash) {
                LOGGER_Debug("Module-_BrokerCall Call crash interrupt, call:"+_call);
                break;
            }
        }
    }
    LOGGER_Info("Module-_BrokerCall Call end, call:"+_call+", remain:"+std::to_string(_loopCount)+", errorCode:"+ERRORCODE_GetCode(retFun)+", message:"+ERRORCODE_GetMessage(retFun));

    //STEP::Replace passParam
    if(!moduleParam.isMember("_param"))
        passParam.swap(_param);

    //STEP::Deal result
    if(_errorKey!=""){
        passParam[_errorKey] = ERRORCODE_GetCode(retFun);
    }
    if(_messageKey!=""){
        passParam[_messageKey] = ERRORCODE_GetMessage(retFun);
    }
    if(_isAdoptFunRet)
        return retFun;
    return ret;
}
#include "Kernel/Module/_BrokerLogger/_BrokerLogger.hpp"

using namespace Module;

ERROR_CODE _BrokerLogger::Error(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _message = TOOLS_ParamRead("_message", "", moduleParam, passParam);
    bool _isPrintParam = TOOLS_ParamRead("_isPrintParam", false, moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerLogger Error _message:" + _message);
    LOGGER_Debug("Module-_BrokerLogger Error _isPrintParam:" + std::to_string(_isPrintParam));

    //STEP::Print log
    if(_isPrintParam){
        LOGGER_Error(_message, moduleParam, passParam);
    } else {
        LOGGER_Error(_message);
    }

    return ret;
}

ERROR_CODE _BrokerLogger::Info(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _message = TOOLS_ParamRead("_message", "", moduleParam, passParam);
    bool _isPrintParam = TOOLS_ParamRead("_isPrintParam", false, moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerLogger Info _message:" + _message);
    LOGGER_Debug("Module-_BrokerLogger Info _isPrintParam:" + std::to_string(_isPrintParam));

    //STEP::Print log
    if(_isPrintParam){
        LOGGER_Info(_message, moduleParam, passParam);
    } else {
        LOGGER_Info(_message);
    }

    return ret;
}

ERROR_CODE _BrokerLogger::Debug(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _message = TOOLS_ParamRead("_message", "", moduleParam, passParam);
    bool _isPrintParam = TOOLS_ParamRead("_isPrintParam", false, moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerLogger Debug _message:" + _message);
    LOGGER_Debug("Module-_BrokerLogger Debug _isPrintParam:" + std::to_string(_isPrintParam));

    //STEP::Print log
    if(_isPrintParam){
        LOGGER_Debug(_message, moduleParam, passParam);
    } else {
        LOGGER_Debug(_message);
    }

    return ret;
}

ERROR_CODE _BrokerLogger::Console(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    std::string _message = TOOLS_ParamRead("_message", "", moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerLogger Console _message:" + _message);

    //STEP::Print log
    LOGGER_Console(_message);

    return ret;
}
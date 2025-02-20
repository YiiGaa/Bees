#include "Kernel/Module/_DataFilling/_DataFilling.hpp"

using namespace Module;

ERROR_CODE _DataFilling::Set(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    Json::Value _setting = TOOLS_ParamRead("_setting", Json::objectValue, moduleParam, passParam);
    bool _isSwitchNullError = TOOLS_ParamRead("_isSwitchNullError", true, moduleParam, passParam);
    LOGGER_Debug("Module-_DataFilling Set _setting:" + _setting.toStyledString());
    LOGGER_Debug("Module-_DataFilling Set _isSwitchNullError:" + std::to_string(_isSwitchNullError));

    //STEP::Traverse filling passParam
    ret = this->Traverse(_setting, passParam, passParam, _isSwitchNullError);
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    return ret;
}
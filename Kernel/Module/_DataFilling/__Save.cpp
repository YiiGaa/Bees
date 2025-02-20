#include "Kernel/Module/_DataFilling/_DataFilling.hpp"

using namespace Module;

ERROR_CODE _DataFilling::Save(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Get setting
    Json::Value _setting = TOOLS_ParamRead("_setting", Json::objectValue, moduleParam, passParam);
    LOGGER_Debug("Module-_DataFilling Save _setting:" + _setting.toStyledString());

    //STEP::Traverse filling passParam
    TOOLS_Lock(dataPoolMutex, lock);
    ret = this->Traverse(_setting, dataPool, passParam, false);
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    return ret;
}
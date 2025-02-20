#include "Kernel/Module/_DataCheck/_DataCheck.hpp"
#include <regex>

using namespace Module;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_DataCheck::_DataCheck(void){
}

ERROR_CODE _DataCheck::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    //try {
        //STEP::Get setting
        bool _isClean = TOOLS_ParamRead("_isClean", false, moduleParam, passParam);
        Json::Value _param = TOOLS_ParamRead("_param", Json::objectValue, moduleParam, passParam);
        LOGGER_Debug("Module-_DataCheck DoStart _isClean:" + std::to_string(_isClean));
        LOGGER_Debug("Module-_DataCheck DoStart _param:" + _param.toStyledString());

        //STEP::Traverse check param
        Json::Value resultParam = Json::objectValue;
        ret = this->Traverse(_param, passParam, resultParam, passParam);
        if(ERRORCODE_IsError(ret)){
            return ret;
        }

        //STEP::Replace result
        if(_isClean)
            passParam = resultParam;
        
//    } catch (std::exception& e){
//        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
//        return ERR_Module__DataCheck_Exception;
//    } catch (...){
//        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
//        return ERR_Module__DataCheck_Exception;
//    }

    return ret;
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _DataCheck::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_DataCheck Clean deal");
        else
            LOGGER_Info("_DataCheck Crash deal");

        //STEP::Do Clean

    } catch (std::exception& e){
        LOGGER_Error("_DataCheck Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_DataCheck Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

//TIPS::Module entrance
ERROR_CODE _DataCheck::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_DataCheck start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_DataCheck end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
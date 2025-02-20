#include "Kernel/Module/_DataFilling/_DataFilling.hpp"

using namespace Module;

Json::Value _DataFilling::dataPool;
std::shared_mutex _DataFilling::dataPoolMutex;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_DataFilling::_DataFilling(void){
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _DataFilling::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_DataFilling Clean deal");
        else
            LOGGER_Info("_DataFilling Crash deal");

        //STEP::Do Clean

    } catch (std::exception& e){
        LOGGER_Error("_DataFilling Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_DataFilling Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

ERROR_CODE _DataFilling::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    try {
        std::string _action = TOOLS_ParamRead("_action", "", moduleParam, passParam);

        //ACTION::set
        if(_action=="set"){
            ret = this->Set(moduleParam, passParam, ret);
        }
        //ACTION::save
        else if(_action=="save"){
            ret = this->Save(moduleParam, passParam, ret);
        } else {
            ret = ERR_Module__DataFilling_Action_Illegal;
        }
    } catch (std::exception& e){
        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
        return ERR_Module__DataFilling_Exception;
    } catch (...){
        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
        return ERR_Module__DataFilling_Exception;
    }

    return ret;
}

//TIPS::Module entrance
ERROR_CODE _DataFilling::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_DataFilling start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_DataFilling end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
#include "Kernel/Module/_BrokerLogger/_BrokerLogger.hpp"

using namespace Module;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_BrokerLogger::_BrokerLogger(void){
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _BrokerLogger::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_BrokerLogger Clean deal");
        else
            LOGGER_Info("_BrokerLogger Crash deal");

        //STEP::Do Clean

    } catch (std::exception& e){
        LOGGER_Error("_BrokerLogger Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_BrokerLogger Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

ERROR_CODE _BrokerLogger::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    try {
        std::string _action = TOOLS_ParamRead("_action", "", moduleParam, passParam);

        if(_action=="info"){
            ret = this->Info(moduleParam, passParam, ret);
        }
        else if(_action=="error"){
            ret = this->Error(moduleParam, passParam, ret);
        }
        else if(_action=="debug"){
            ret = this->Debug(moduleParam, passParam, ret);
        }
        else if(_action=="console"){
            ret = this->Console(moduleParam, passParam, ret);
        }
        else {
            ret = ERR_Module__BrokerLogger_Action_Illegal;
        }
    } catch (std::exception& e){
        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
        return ERR_Module__BrokerLogger_Exception;
    } catch (...){
        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
        return ERR_Module__BrokerLogger_Exception;
    }

    return ret;
}

//TIPS::Module entrance
ERROR_CODE _BrokerLogger::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_BrokerLogger start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_BrokerLogger end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
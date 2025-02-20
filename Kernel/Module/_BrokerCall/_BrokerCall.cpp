#include "Kernel/Module/_BrokerCall/_BrokerCall.hpp"

using namespace Module;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_BrokerCall::_BrokerCall(void){
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _BrokerCall::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_BrokerCall Clean deal");
        else
            LOGGER_Info("_BrokerCall Crash deal");

        //STEP::Do Clean
        this->isCrash = true;
    } catch (std::exception& e){
        LOGGER_Error("_BrokerCall Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_BrokerCall Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

ERROR_CODE _BrokerCall::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    try {
        std::string _action = TOOLS_ParamRead("_action", "", moduleParam, passParam);
        innerParam_t* innerParam = NULL;

        if(_action == "call"){
            ret = this->DataCreate(moduleParam, passParam, innerParam, ret);
            ret = this->Call(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else if(_action == "end"){
            ret = this->DataGet(moduleParam, passParam, innerParam, ret);
            ret = this->End(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else{
            ret = ERR_Module__BrokerCall_Action_Illegal;
        }

    } catch (std::exception& e){
        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
        return ERR_Module__BrokerCall_Exception;
    } catch (...){
        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
        return ERR_Module__BrokerCall_Exception;
    }

    return ret;
}

//TIPS::Module entrance
ERROR_CODE _BrokerCall::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_BrokerCall start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_BrokerCall end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
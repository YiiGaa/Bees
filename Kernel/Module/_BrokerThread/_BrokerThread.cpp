#include "Kernel/Module/_BrokerThread/_BrokerThread.hpp"

using namespace Module;

std::map<std::string, _BrokerThread::innerParam_t*> _BrokerThread::innerParamMap;
std::condition_variable _BrokerThread::innerParamCondWait;
std::mutex _BrokerThread::innerParamMutex;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_BrokerThread::_BrokerThread(void){
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _BrokerThread::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_BrokerThread Clean deal");
        else
            LOGGER_Info("_BrokerThread Crash deal");

        //STEP::Do Clean

    } catch (std::exception& e){
        LOGGER_Error("_BrokerThread Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_BrokerThread Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

ERROR_CODE _BrokerThread::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    try {
        std::string _action = TOOLS_ParamRead("_action", "", moduleParam, passParam);
        innerParam_t* innerParam = NULL;

        if(_action=="create"){
            ret = this->DataCreate(moduleParam, passParam, innerParam, ret);
            ret = this->Create(moduleParam, passParam, innerParam, ret);
        }
        else if(_action=="wait"){
            ret = this->DataGet(moduleParam, passParam, innerParam, ret);
            ret = this->Wait(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else if(_action=="wait all"){
            ret = this->WaitAll(moduleParam, passParam, innerParam, ret);
        } else {
            ret = ERR_Module__BrokerThread_Action_Illegal;
        }
    } catch (std::exception& e){
        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
        return ERR_Module__BrokerThread_Exception;
    } catch (...){
        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
        return ERR_Module__BrokerThread_Exception;
    }

    return ret;
}

//TIPS::Module entrance
ERROR_CODE _BrokerThread::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_BrokerThread start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_BrokerThread end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
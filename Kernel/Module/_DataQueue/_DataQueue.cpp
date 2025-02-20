#include "Kernel/Module/_DataQueue/_DataQueue.hpp"
#include <sys/time.h>
#include <chrono>

using namespace Module;

//TIPS::Constructor function
//Each module will only have 1 object (created at program startup), please fully consider the impact of multithreading
_DataQueue::_DataQueue(void){
}

//TIPS::Program end or crash will call this
//Please note the impact of multithreading
void _DataQueue::Crash(int isClean = 0){
    try{
        //STEP::Print log
        if(isClean)
            LOGGER_Info("_DataQueue Clean deal");
        else
            LOGGER_Info("_DataQueue Crash deal");

        //STEP::Do Clean

    } catch (std::exception& e){
        LOGGER_Error("_DataQueue Clean exception:"+std::string(e.what()));
    } catch (...){
        LOGGER_Error("_DataQueue Clean unknown exception, maybe Segmentation fault (core dumped)");
    }
}

ERROR_CODE _DataQueue::DoStart(Json::Value& moduleParam, Json::Value& passParam, ERROR_CODE ret){
    try {
        std::string _action = TOOLS_ParamRead("_action", "", moduleParam, passParam);
        innerParam_t* innerParam = NULL;

        if(_action=="create"){
            ret = this->DataCreate(moduleParam, passParam, innerParam, ret);
        }
        else if(_action=="push"){
            ret = this->DataGet(moduleParam, passParam, innerParam, ret);
            ret = this->Push(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else if(_action=="pull"){
            ret = this->DataGet(moduleParam, passParam, innerParam, ret);
            ret = this->Pull(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else if(_action=="pop"){
            ret = this->DataGet(moduleParam, passParam, innerParam, ret);
            ret = this->PullOnlyPop(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else if(_action=="delete"){
            //TIPS::Delete twice for clean useCount
            ret = this->DataGet(moduleParam, passParam, innerParam, ret);
            ret = this->End(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
            ret = this->DataDelete(moduleParam, passParam, innerParam, ret);
        }
        else{
            ret = ERR_Module__DataQueue_Action_Illegal;
        }
    } catch (std::exception& e){
        LOGGER_Error(std::string(e.what()), moduleParam, passParam);
        return ERR_Module__DataQueue_Exception;
    } catch (...){
        LOGGER_Error("unknown exception, maybe Segmentation fault (core dumped)", moduleParam, passParam);
        return ERR_Module__DataQueue_Exception;
    }

    return ret;
}

//TIPS::Module entrance
ERROR_CODE _DataQueue::Start(Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_Debug("Module-_DataQueue start", moduleParam, passParam);
    ERROR_CODE ret = ERR_OK;
    ret = this->DoStart(moduleParam, passParam, ret);
    LOGGER_Debug("Module-_DataQueue end, errorCode:"+ERRORCODE_GetCode(ret)+", message:"+ERRORCODE_GetMessage(ret), moduleParam, passParam);
    return ret;
}
#include "Kernel/Module/_DataQueue/_DataQueue.hpp"
#include <sys/time.h>
#include <chrono>
#include <limits>

using namespace Module;

ERROR_CODE _DataQueue::Push(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    Json::Value _data = TOOLS_ParamRead("_data", Json::nullValue, moduleParam, passParam);
    int _timeout = TOOLS_ParamRead("_timeout", 0, moduleParam, passParam);
    std::string _timeoutCall = TOOLS_ParamRead("_timeoutCall", "", moduleParam, passParam);
    bool _isFullWait = TOOLS_ParamRead("_isFullWait", true, moduleParam, passParam);
    LOGGER_Debug("Module-_BrokerCall Push _identity:" + _identity);
    LOGGER_Debug("Module-_BrokerCall Push _data:" + _data.toStyledString());
    LOGGER_Debug("Module-_BrokerCall Push _timeout:" + std::to_string(_timeout));
    LOGGER_Debug("Module-_BrokerCall Push _timeoutCall:" + _timeoutCall);
    LOGGER_Debug("Module-_BrokerCall Push _isFullWait:" + std::to_string(_isFullWait));

    //STEP::Check _timeoutCall
    if(_timeoutCall!=""){
        if(!TOOLS_CallFunctionCheck(_timeoutCall)){
            LOGGER_Error("_DataQueue Push timeout callback function lack _timeoutCall:"+_timeoutCall);
            return ERR_Module__DataQueue_Push_TimoutOutCall_Lack;
        }
    }

    //STEP::Lock
    TOOLS_Lock(innerParam->queueMutex, lock);

    //STEP::Push data
    queueData_t *queueData = new queueData_t();
    innerParam->queue.push_back(queueData);
    innerParam->idCount = innerParam->idCount>=std::numeric_limits<int>::max()?0:innerParam->idCount+1;
    queueData->id = innerParam->idCount;
    queueData->_data.swap(_data);
    queueData->isUse = false;
    innerParam->queuePush.notify_one();

    //WHEN::Full not wait
    if(!innerParam->isCallEnd && !_isFullWait){
        if(innerParam->_queueMaxLen>=0 && innerParam->queue.size()>innerParam->_queueMaxLen){
            LOGGER_Info("_DataQueue Push full erase, _identity:"+_identity+", data id:"+std::to_string(innerParam->queue[0]->id));
            innerParam->queue.erase(innerParam->queue.begin());
        }
        return ret;
    }

    //STEP::Check length
    while(!innerParam->isCallEnd && _isFullWait && innerParam->_queueMaxLen>=0 && innerParam->queue.size()>innerParam->_queueMaxLen){
        if(_timeout<=0) {
            LOGGER_Info("_DataQueue Push full wait, _identity:" + _identity);
            innerParam->queuePull.wait(lock);
        }else{
            LOGGER_Info("_DataQueue Push full wait(timeout), _identity:" + _identity+", _timeoutCall:"+_timeoutCall);
            if(innerParam->queuePull.wait_for(lock, std::chrono::seconds(_timeout)) == std::cv_status::timeout){
                if(_timeoutCall!=""){
                    lock.unlock();
                    TOOLS_CallFunction(_timeoutCall, passParam);
                    lock.lock();
                }
            }
        }
    }

    //WHEN::Call end
    if(innerParam->isCallEnd){
        LOGGER_Info("_DataQueue Push called end, _identity:" + _identity);
        return ERR_Module__DataQueue_Push_End;
    }

    return ret;
}
#include "Kernel/Module/_DataQueue/_DataQueue.hpp"

using namespace Module;

ERROR_CODE _DataQueue::Pull(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    std::string _resultKey = TOOLS_ParamRead("_resultKey", "", moduleParam, passParam);
    bool _isPop = TOOLS_ParamRead("_isPop", true, moduleParam, passParam);
    std::string _dataIdKey = TOOLS_ParamRead("_dataIdKey", "dataId", moduleParam, passParam);
    int _timeout = TOOLS_ParamRead("_timeout", 0, moduleParam, passParam);
    std::string _timeoutCall = TOOLS_ParamRead("_timeoutCall", "", moduleParam, passParam);
    bool _isEmptyWait = TOOLS_ParamRead("_isEmptyWait", true, moduleParam, passParam);
    LOGGER_Debug("Module-_DataQueue Pull _identity:" + _identity);
    LOGGER_Debug("Module-_DataQueue Pull _resultKey:" + _resultKey);
    LOGGER_Debug("Module-_DataQueue Pull _isPop:" + std::to_string(_isPop));
    LOGGER_Debug("Module-_DataQueue Pull _dataIdKey:" + _dataIdKey);
    LOGGER_Debug("Module-_DataQueue Pull _timeout:" + std::to_string(_timeout));
    LOGGER_Debug("Module-_DataQueue Pull _timeoutCall:" + _timeoutCall);
    LOGGER_Debug("Module-_DataQueue Pull _isEmptyWait:" + std::to_string(_isEmptyWait));

    //STEP::Check _timeoutCall
    if(_timeoutCall!=""){
        if(!TOOLS_CallFunctionCheck(_timeoutCall)){
            LOGGER_Error("_DataQueue Pull timeout callback function lack _timeoutCall:"+_timeoutCall);
            return ERR_Module__DataQueue_Pull_TimoutOutCall_Lack;
        }
    }

    //STEP::Lock
    TOOLS_Lock(innerParam->queueMutex, lock);

    //STEP::Get index not in use
    std::size_t index = 0;
    while(!innerParam->isCallEnd) {
        //STEP-IN::Get index
        bool isEmpty = true;
        for (const auto &data: innerParam->queue) {
            if (!data->isUse) {
                isEmpty = false;
                break;
            }
            index++;
        }

        //WHEN-IN::Get success
        if(!isEmpty){
            break;
        }

        //WHEN-IN::Empty ant not wait
        if (isEmpty && !_isEmptyWait) {
            LOGGER_Debug("_DataQueue Pull empty, _identity:" + _identity);
            return ERR_Module__DataQueue_Pull_Empty;
        }

        //WHEN-IN::Empty ant wait
        if(_timeout<=0) {
            LOGGER_Info("_DataQueue Pull empty wait, _identity:" + _identity);
            innerParam->queuePush.wait(lock);
        }else{
            LOGGER_Info("_DataQueue Pull empty wait(timeout), _identity:" + _identity+", _timeoutCall:"+_timeoutCall);
            if(innerParam->queuePush.wait_for(lock, std::chrono::seconds(_timeout)) == std::cv_status::timeout){
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
        LOGGER_Info("_DataQueue Pull called end, _identity:" + _identity);
        return ERR_Module__DataQueue_Pull_End;
    }

    //STEP::Get data
    if(_resultKey!="" || innerParam->queue[index]->_data.type() != Json::ValueType::objectValue){
        passParam[_resultKey] = innerParam->queue[index]->_data;
    }else{
        passParam = innerParam->queue[index]->_data;
    }
    if(!_isPop)
        passParam[_dataIdKey] = innerParam->queue[index]->id;

    //STEP::Pop queue
    if(_isPop){
        LOGGER_Info("_DataQueue Pull erase data, _identity:" + _identity);
        queueData_t *queueData = innerParam->queue[index];
        innerParam->queue.erase(innerParam->queue.begin() + index);
        innerParam->queuePull.notify_one();
        delete queueData;
    } else {
        LOGGER_Info("_DataQueue Pull use data, _identity:" + _identity);
        innerParam->queue[index]->isUse = true;
    }

    return ret;
}

ERROR_CODE _DataQueue::PullOnlyPop(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    int _id = TOOLS_ParamRead("_id", -1, moduleParam, passParam);
    bool _isNotFoundError = TOOLS_ParamRead("_isNotFoundError", false, moduleParam, passParam);
    LOGGER_Debug("Module-_DataQueue PullOnlyPop _identity:" + _identity);
    LOGGER_Debug("Module-_DataQueue PullOnlyPop _id:" + std::to_string(_id));
    LOGGER_Debug("Module-_DataQueue PullOnlyPop _isNotFoundError:" + std::to_string(_isNotFoundError));

    //STEP::Lock
    TOOLS_Lock(innerParam->queueMutex, lock);

    //STEP::Find index
    std::size_t index = 0;
    bool isFound = false;
    for (const auto &data: innerParam->queue) {
        if(data->isUse && _id==-1) {
            isFound = true;
            break;
        } else if(data->isUse && data->id == _id){
            isFound = true;
            break;
        }
        index++;
    }

    //STEP::Pop queue
    if(isFound){
        LOGGER_Info("_DataQueue PullOnlyPop erase data, _identity:" + _identity);
        innerParam->queue.erase(innerParam->queue.begin() + index);
        queueData_t *queueData = innerParam->queue[index];
        innerParam->queuePull.notify_one();
        delete queueData;
    } else if(_isNotFoundError){
        LOGGER_Error("_DataQueue PullOnlyPop erase false, _identity:" + _identity);
        return ERR_Module__DataQueue_PullOnlyPop_Found_Block;
    }

    return ret;
}
#include "Kernel/Module/_DataQueue/_DataQueue.hpp"


using namespace Module;

ERROR_CODE _DataQueue::End(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret) {
    //STEP::Check last step result
    if (ERRORCODE_IsError(ret)) {
        return ret;
    }

    //STEP::Get setting
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    LOGGER_Debug("Module-_DataQueue Pull _identity:" + _identity);

    //STEP::Lock
    TOOLS_Lock(innerParam->queueMutex, lock);
    innerParam->isCallEnd = true;

    //STEP::Notify wait block
    LOGGER_Info("_DataQueue End call end, _identity:" + _identity);
    innerParam->queuePull.notify_all();
    innerParam->queuePush.notify_all();

    return ret;
}
#include "Kernel/Module/_BrokerCall/_BrokerCall.hpp"

using namespace Module;

ERROR_CODE _BrokerCall::End(Json::Value& moduleParam, Json::Value& passParam, innerParam_t*& innerParam, ERROR_CODE ret){
    //STEP::Check last step result
    if(ERRORCODE_IsError(ret)){
        return ret;
    }

    //STEP::Call end
    std::string _identity = TOOLS_ParamRead("_identity", "", moduleParam, passParam);
    innerParam->isCallEnd.store(true);
    LOGGER_Info("Module-_BrokerCall End call end, _identity:"+_identity);

    return ret;
}
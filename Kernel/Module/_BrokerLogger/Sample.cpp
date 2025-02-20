#include "Kernel/Module/_BrokerLogger/_BrokerLogger.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
    {"_BrokerLogger", new Module::_BrokerLogger()},
};
ERROR_CODE START_MODULE(std::string module, Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_LocateAdd("M-"+module);
    ERROR_CODE ret = (moduleMap[module])->Start(moduleParam, passParam);
    LOGGER_LocateDelete();
    return ret;
}
/* The same function as "Kernel/Module/Module.hpp" */

int main(int argc, char *argv[]){
    TOOLS_EndCrashListen(moduleMap);
    Json::Value passParam = Json::objectValue;
    Json::Value moduleParam = Json::objectValue;
    ERROR_CODE ret = ERR_OK;
    Json::Reader reader;

    //STEP::Init log
    LOGGER_InitSample();

    //STEP::Make passParam for test
    reader.parse(R"(
        {}
    )", passParam);

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "info",
            "_message": "**test info log...",
            "_isPrintParam": true,
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerLogger", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "error",
            "_message": "**test error log...",
            "_isPrintParam": true,
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerLogger", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "console",
            "_message": "**test console log..."
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerLogger", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "debug",
            "_message": "**test debug log..."
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerLogger", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

end:
    //STEP::Print result
    std::cout<<std::endl<<"errorCode:"<<ERRORCODE_GetCode(ret)<<", message:"<<ERRORCODE_GetMessage(ret)<<std::endl;
    std::cout<<"passParam:"<<passParam.toStyledString()<<std::endl;

    //STEP::Clean module and close log
    TOOLS_EndClean();

    return ERRORCODE_IsError(ret)?-1:0;
}
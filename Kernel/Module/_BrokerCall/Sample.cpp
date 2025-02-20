#include "Kernel/Module/_BrokerCall/_BrokerCall.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
        {"_BrokerCall", new Module::_BrokerCall()},
};
ERROR_CODE START_MODULE(std::string module, Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_LocateAdd("M-"+module);
    ERROR_CODE ret = (moduleMap[module])->Start(moduleParam, passParam);
    LOGGER_LocateDelete();
    return ret;
}
/* The same function as "Kernel/Module/Module.hpp" */

ERROR_CODE TestCallBack(Json::Value &passParam){
    Json::Value moduleParam = Json::objectValue;
    Json::Reader reader;
    ERROR_CODE ret = ERR_OK;

    //STEP::Print log
    passParam["test"] = "234";
    std::cout<<std::endl<<"* Call TestCallBack:"<<passParam.toStyledString()<<std::endl;

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"end",
            "_identity":"test"
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerCall", moduleParam, passParam);
    TOOLS_JsonClean(moduleParam);

    return ret;
}

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
        {
            "key-pass":"value"
        }
    )", passParam);

    //STEP::Mark function for test
    TOOLS_CallFunctionMark("TestCallBack",TestCallBack);

    //STEP::Call module
    reader.parse(R"(
        {
            "_action":"call",
            "_identity":"test",
            "_call":"TestCallBack",
            "_loopCount":3,
            "_sleepTime":3000000,
            "_isErrorStop":true,
            "_param":{
                "key":"value"
            }
    )", moduleParam);
    ret = START_MODULE("_BrokerCall", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

end:
    //STEP::Print result
    std::cout<<std::endl<<std::endl<<"errorCode:"<<ERRORCODE_GetCode(ret)<<", message:"<<ERRORCODE_GetMessage(ret)<<std::endl;
    std::cout<<"passParam:"<<passParam.toStyledString()<<std::endl;

    //STEP::Clean module and close log
    TOOLS_EndClean();

    return ERRORCODE_IsError(ret)?-1:0;
}
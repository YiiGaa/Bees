#include "Kernel/Module/_BrokerThread/_BrokerThread.hpp"
#include "Kernel/Common/Common.hpp"
#include <iostream>
#include <string>

/* The same function as "Kernel/Module/Module.hpp" */
std::map<std::string, Module::Link*> moduleMap = {
    {"_BrokerThread", new Module::_BrokerThread()},
};
ERROR_CODE START_MODULE(std::string module, Json::Value& moduleParam, Json::Value& passParam){
    LOGGER_LocateAdd("M-"+module);
    ERROR_CODE ret = (moduleMap[module])->Start(moduleParam, passParam);
    LOGGER_LocateDelete();
    return ret;
}
/* The same function as "Kernel/Module/Module.hpp" */

ERROR_CODE Test_1(Json::Value& param){
    LOGGER_Console("**Test_1 start:: "+param.toStyledString());
    int count = 10;
    while(count--){
        LOGGER_Console("**Test_1 loop:: "+std::to_string(count));
        sleep(1);
    }
    return ERR_OK;
}

ERROR_CODE Test_2(Json::Value& param){
    LOGGER_Console("**Test_2 start:: "+param.toStyledString());
    int count = 7;
    while(count--){
        LOGGER_Console("**Test_2 loop:: "+std::to_string(count));
        sleep(1);
    }
    return ERR_OK;
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
            "pass_1":{
                "key_1":"test_1"
            },
            "pass_2":{
                "key_2":"test_2"
            }
        }
    )", passParam);

    //STEP::Mark thread for test
    TOOLS_CallThreadMark("Test_1", Test_1);
    TOOLS_CallThreadMark("Test_2", Test_2);

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "create", 
            "_identity": "Test_1",
            "_call": "Test_1",
            "_param": {
                "key":"value"
            }
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerThread", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "create", 
            "_identity": "Test_2",
            "_call": "Test_2"
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerThread", moduleParam, passParam);
    if(ERRORCODE_IsError(ret)){
        goto end;
    }

    //STEP::Call module
    reader.parse(R"(
        {
            "_action": "wait",
            "_identity": "Test_2"
        }
    )", moduleParam);
    ret = START_MODULE("_BrokerThread", moduleParam, passParam);
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
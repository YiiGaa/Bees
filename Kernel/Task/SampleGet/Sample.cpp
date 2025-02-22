#include "Kernel/Task/SampleGet/SampleGet.hpp"
#include "Kernel/Common/Common.hpp"
#include "Kernel/Task/Task.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]){
    TOOLS_EndCrashListen(Module::moduleMap);
    Json::Value passParam = Json::objectValue;
    Json::Value moduleParam = Json::objectValue;
    Json::Reader reader;

    //STEP::Init log
    LOGGER_InitSample();
    TOOLS_CallFunctionMark(Task::taskFunctionMap);

    //STEP::Make passParam for test
    reader.parse(R"(
        {}
    )", passParam);

    //STEP::Call Start
    ERROR_CODE ret = Task::SampleGet::Start(passParam);

    //STEP::Print result
    std::cout<<std::endl<<"errorCode:"<<ERRORCODE_GetCode(ret)<<", message:"<<ERRORCODE_GetMessage(ret)<<std::endl;
    std::cout<<"passParam:"<<passParam.toStyledString()<<std::endl;

    //STEP::Clean module and close log
    TOOLS_EndClean();

    return ERRORCODE_IsError(ret)?-1:0;
}
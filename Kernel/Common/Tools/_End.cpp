#include "Kernel/Common/Tools/Tools.hpp"
#include <csignal>

//TIPS::The program exits normally, then performs cleanup
//>>param::Module map. When a crash occurs, the list of modules to be cleaned
//>>return::void
void Common::Tools::EndCrashListen(std::map<std::string, Module::Link*>* param){
    //STEP::Listen crash signal
    signal(SIGABRT, EndCrashListen_Crash);  //Abnormal termination
    signal(SIGFPE, EndCrashListen_Crash);   //Wrong arithmetic operation
    signal(SIGILL, EndCrashListen_Crash);   //Illegal instructions
    signal(SIGINT, EndCrashListen_Crash);   //Interrupt(ctrl+c)
    signal(SIGSEGV, EndCrashListen_Crash);  //Illegal access to memory
    signal(SIGTERM, EndCrashListen_Crash);  //Termination request

    //STEP::Mark module map
    EndCrashListen_Map = param;
    return;
}
std::map<std::string, Module::Link*> *Common::Tools::EndCrashListen_Map;
void Common::Tools::EndCrashListen_Crash(int sign){
    if(sign == SIGABRT||    //Abnormal termination
       sign == SIGFPE||     //Wrong arithmetic operation
       sign == SIGILL||     //Illegal instructions
       sign == SIGINT||     //Interrupt(ctrl+c)
       sign == SIGSEGV||    //Illegal access to memory
       sign == SIGTERM)     //Termination request
    {
        //STEP::Clean module
        for(std::map<std::string, Module::Link*>::iterator iter = EndCrashListen_Map->begin(); iter != EndCrashListen_Map->end(); iter++) {
            iter->second->Crash();
        }

        //STEP::Close log
        LOGGER_Close();

        //STEP::Cancel the capture of this signal, When this signal is triggered again, the system will directly process this signal
        signal(sign,SIG_DFL);

        //STEP::Terminate the program immediately
        std::abort();
    }
    signal(sign,SIG_DFL);
}

//TIPS::The program exits normally, then performs cleanup
//>>return::void
void Common::Tools::EndClean(){
    //STEP::Clean all module
    for(std::map<std::string, Module::Link*>::iterator iter = EndCrashListen_Map->begin(); iter != EndCrashListen_Map->end(); iter++) {
        iter->second->Crash(1);
    }

    //STEP::Close log
    LOGGER_Close();
}
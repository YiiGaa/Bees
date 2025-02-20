#ifndef   _MODULE__BrokerThread_HPP_
#define   _MODULE__BrokerThread_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <map>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>

namespace Module{
    class _BrokerThread: public Link{
        //TIPS::Variables in the module
        private:
            typedef struct innerParam {
                int useCount;
                Json::Value _param;
                bool isEnd;
                std::string _call;
                std::string _identity;
                std::mutex mutex;
                std::condition_variable condWait;

                innerParam(){
                    _param = Json::objectValue;
                    useCount = 0;
                    _call = "";
                    _identity = "";
                }
            } innerParam_t;
            static std::map<std::string, innerParam_t*> innerParamMap;
            static std::condition_variable innerParamCondWait;
            static std::mutex innerParamMutex;

        //TIPS::Action processing function
        private:
            static void Create_Do(std::string);
            static ERROR_CODE DataDeleteThread(innerParam_t*&);

            ERROR_CODE Create(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE Wait(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE WaitAll(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataCreate(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataGet(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataDelete(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);
        
        public:
            void Crash(int);
            _BrokerThread();
            ERROR_CODE Start(Json::Value&, Json::Value&);
    };
}

#endif
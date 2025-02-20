#ifndef   _MODULE__DataQueue_HPP_
#define   _MODULE__DataQueue_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>

namespace Module{
    class _DataQueue: public Link{
        //TIPS::Variables in the module
        private:
            typedef struct queueData {
                int id;
                bool isUse;
                Json::Value _data;
            } queueData_t;

            typedef struct innerParam {
                std::vector<queueData_t*> queue;
                int _queueMaxLen;
                std::mutex queueMutex;
                std::condition_variable queuePush;
                std::condition_variable queuePull;
                int idCount;
                int useCount;
                bool isCallEnd;

                innerParam(){
                    useCount = 0;
                    idCount = 0;
                    _queueMaxLen = -1;
                    isCallEnd = false;
                }
            } innerParam_t;
            std::map<std::string, innerParam_t*> innerParamMap;
            std::shared_mutex innerParamMutex;
            bool isCrash = false;

        //TIPS::Action processing function
        private:
            ERROR_CODE DataCreate(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataGet(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataDelete(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE Push(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE Pull(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE PullOnlyPop(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE End(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);
        
        public:
            _DataQueue();
            void Crash(int);
            ERROR_CODE Start(Json::Value&, Json::Value&);
    };
}

#endif
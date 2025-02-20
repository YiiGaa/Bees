#ifndef   _MODULE__BrokerCall_HPP_
#define   _MODULE__BrokerCall_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <atomic>
#include <map>
#include <mutex>
#include <shared_mutex>

namespace Module{
    class _BrokerCall: public Link{
        //TIPS::Variables in the module
        private:
            typedef struct innerParam {
                std::atomic<bool> isCallEnd;
                int useCount;

                innerParam(){
                    isCallEnd = false;
                    useCount = 0;
                }
            } innerParam_t;
            std::map<std::string, innerParam_t*> innerParamMap;
            std::shared_mutex innerParamMutex;
            bool isCrash = false;

        //TIPS::Action processing function
        private:
            ERROR_CODE Call(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE End(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataCreate(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataGet(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DataDelete(Json::Value&, Json::Value&, innerParam_t*&, ERROR_CODE);
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);

        public:
            void Crash(int);
            _BrokerCall();
            ERROR_CODE Start(Json::Value&, Json::Value&);
    };
}

#endif
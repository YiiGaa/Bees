#ifndef   _MODULE__BrokerLogger_HPP_
#define   _MODULE__BrokerLogger_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <map>
#include <vector>

namespace Module{
    class _BrokerLogger: public Link{
        //TIPS::Variables in the module
        private:

        //TIPS::Action processing function
        private:
            ERROR_CODE Error(Json::Value&, Json::Value&, ERROR_CODE);
            ERROR_CODE Info(Json::Value&, Json::Value&, ERROR_CODE);
            ERROR_CODE Debug(Json::Value&, Json::Value&, ERROR_CODE);
            ERROR_CODE Console(Json::Value&, Json::Value&, ERROR_CODE);
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);
        
        public:
            void Crash(int isCrash);
            _BrokerLogger();
            ERROR_CODE Start(Json::Value&, Json::Value&);
    };
}

#endif
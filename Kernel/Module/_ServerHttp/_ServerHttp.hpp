#ifndef   _MODULE__ServerHttp_HPP_
#define   _MODULE__ServerHttp_HPP_

#include "Kernel/Common/Common.hpp"
#include "Kernel/Module/Link.hpp"
#include <json/json.h>
#include <map>
#include <string>

namespace Module{
    class _ServerHttp: public Link{
        private:
            std::string URLEncode(const std::string& value);

        //TIPS::Action processing function
        private:
            ERROR_CODE DoStart(Json::Value&, Json::Value&, ERROR_CODE);

        public: 
            _ServerHttp();
            void Crash(int);
            ERROR_CODE Start(Json::Value&, Json::Value&);           
    };
}

#endif
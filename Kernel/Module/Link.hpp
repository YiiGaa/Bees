#ifndef   _MODULE_Link_HPP_
#define   _MODULE_Link_HPP_

#include "Kernel/Common/Common.hpp"

namespace Module{
    class Link {
        public:
            virtual ERROR_CODE Start(Json::Value& moduleParam, Json::Value& passParam){return ERR_OK;};
            virtual void Crash(int isClean = 0){return;};
    };
}

#endif
#pragma once
#pragma clang system_header

#include "../Record.h"
#include "../Util.h"

namespace plog
{
    class PLOG_LINKAGE IAppender
    {
    public:
        virtual ~IAppender()
        {
        }

        virtual void write(const Record& record) = 0;
    };
}

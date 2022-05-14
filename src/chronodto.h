#pragma once
#include "config.h"

namespace chronoscopist
{
    enum messagetype
    {
        none = 0,
        ping = 1,
        pong,
        lock,
        unlock
    };

    struct chrmessage
    {
        // chrmessage() = default;
        // chrmessage(const chrmessage &msg);
        static chrmessage generate_message(const messagetype msgtype, const char* text);

        int version = VERSION_1;
        messagetype type;
        char text[MAX_MESSAGE_TEXT];
    };
}

#pragma once
#define VERSION_1 100000001
#define MAX_MESSAGE_TEXT 1024

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

    struct message
    {
        int version = VERSION_1;
        messagetype type;
        char text[MAX_MESSAGE_TEXT];
    };
}

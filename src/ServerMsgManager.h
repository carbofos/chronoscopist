#pragma once
#include <queue>
#include "chronodto.h"

class ServerMsgManager
{
    public: 
        static void start();
        static void queue_message(const chronoscopist::messagetype msgtype, const char* text);
        static std::queue<chronoscopist::message> messages_in;
        static std::queue<chronoscopist::message> messages_out;
};


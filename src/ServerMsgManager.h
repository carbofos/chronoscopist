#pragma once
#include <queue>
#include <list>
#include <string>

#include "chronodto.h"
#include "ChronoServerConnection.h"

class ServerMsgManager
{
    public: 
        static void start();
        static void add_connection(ChronoServerConnection_ptr new_connection_ptr);
        static void queue_message_to_all(const chronoscopist::messagetype msgtype, const char* text);
        static void process_received_message(const chronoscopist::chrmessage msg, std::string ip);

    private:
        static std::list<ChronoServerConnection_ptr> chronoconnections;
};

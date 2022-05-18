#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include "string.h"

#include "ServerMsgManager.h"
#include "chronodto.h"
#include "db.h"
#include "common.h"

// TODO: Надо определиться как менеджер сообщений будет управляться с очередью подключений из ChronoServerConnection,
// каждое из которых является отдельным клиентом

using namespace std::chrono_literals;

void ServerMsgManager::queue_message_to_all(const chronoscopist::messagetype msgtype, const char* text)
{
    auto msg = chronoscopist::chrmessage::generate_message(msgtype, text);
    std::cout << "chronoconnections len: " << chronoconnections.size() << std::endl;
    for (auto connection : chronoconnections)
        connection->queue_tosend_push_message(msg);
}

void ServerMsgManager::process_received_message(const chronoscopist::chrmessage msg, std::string ip)
{
    if (msg.version != VERSION_1)
        {
            std::cerr << "Ignored message with incorrect version" << std::endl;
            return;
        }

    switch (msg.type)
    {
        case chronoscopist::messagetype::pong:
        {
            std::cout << "Recevied PONG from " << ip << std::endl;
            std::ostringstream query_oss;
            query_oss << "INSERT INTO online(ip, time) VALUES(" << ip_to_long(ip) << ", NOW() )";
            std::cout << query_oss.str() << std::endl;
            auto result = Db::exec( query_oss.str() );
            if (result)
                std::cerr << "Mysql error: " << Db::mysql_error_msg << std::endl;
            break;
        }
        default:
        {
            std::cout << "Recevied unhandled msg type: " << msg.type << " from " << ip << std::endl;
            break;
        }
    }

}

void ServerMsgManager::start()
{
    while (true)
    {
        std::cout << "Queueing new message" << std::endl;
        queue_message_to_all(chronoscopist::messagetype::ping, "Ping");
        for (auto connection : chronoconnections)
            connection->send_messages();
        std::this_thread::sleep_for(10000ms);
    }
}

void ServerMsgManager::add_connection(ChronoServerConnection_ptr new_connection_ptr)
{
    ServerMsgManager::chronoconnections.emplace_back(new_connection_ptr);
}

std::list<ChronoServerConnection_ptr> ServerMsgManager::chronoconnections;

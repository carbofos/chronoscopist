#include <iostream>
#include <chrono>
#include <thread>
#include "string.h"
#include "ServerMsgManager.h"
#include "chronodto.h"

// TODO: Надо определиться как менеджер сообщений будет управляться с очередью подключений из ChronoServerConnection,
// каждое из которых является отдельным клиентом

using namespace std::chrono_literals;

void ServerMsgManager::queue_message_to_all(const chronoscopist::messagetype msgtype, const char* text)
{
    auto msg = chronoscopist::chrmessage::generate_message(msgtype, text);
    std::cout << "chronoconnections len: " << chronoconnections.size() << std::endl;
    for (auto connection : chronoconnections)
    {
        connection->queue_tosend_push_message(msg);
        connection->send_messages();
    }
}

void ServerMsgManager::start()
{
    while (true)
    {
        std::cout << "Queueing new message" << std::endl;
        queue_message_to_all(chronoscopist::messagetype::ping, "Ping");
        std::this_thread::sleep_for(10000ms);
    }
}

void ServerMsgManager::add_connection(ChronoServerConnection_ptr new_connection_ptr)
{
    ServerMsgManager::chronoconnections.emplace_back(new_connection_ptr);
}

std::list<ChronoServerConnection_ptr> ServerMsgManager::chronoconnections;

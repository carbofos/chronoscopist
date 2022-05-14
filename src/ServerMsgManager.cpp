#include <iostream>
#include <chrono>
#include <thread>
#include "string.h"
#include "ServerMsgManager.h"
#include "chronodto.h"

// TODO: Надо определиться как менеджер сообщений будет управляться с очередью подключений из ChronoServerConnection,
// каждое из которых является отдельным клиентом


using namespace std::chrono_literals;

void ServerMsgManager::queue_message(const chronoscopist::messagetype msgtype, const char* text)
{
        chronoscopist::message msg;
        msg.type = msgtype;
        msg.version = VERSION_1;
        strncpy(msg.text, text, sizeof(msg.text));
        messages_out.push(std::move(msg));
}

void ServerMsgManager::start()
{
    while (true)
    {
        std::cout << "Queueing new message" << std::endl;
        queue_message(chronoscopist::messagetype::ping, "Ping");
        std::this_thread::sleep_for(60000ms);
    }

}

std::queue<chronoscopist::message> ServerMsgManager::messages_in;
std::queue<chronoscopist::message> ServerMsgManager::messages_out;

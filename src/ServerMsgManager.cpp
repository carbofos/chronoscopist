#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include "string.h"

#include "ServerMsgManager.h"
#include "chronodto.h"
#include "db.h"
#include "common.h"
#include "config.h"

using namespace std::chrono_literals;

void ServerMsgManager::queue_message_to_all(const chronoscopist::messagetype &msgtype, const char* text)
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

void ServerMsgManager::loop()
{
    while (true)
    {
        std::cout << "Queueing new message" << std::endl;
        queue_message_to_all(chronoscopist::messagetype::ping, "Ping");
        check_limits();
        for (auto connection : chronoconnections)
            connection->send_messages();
        std::this_thread::sleep_for(60000ms);
    }
}

void ServerMsgManager::check_limits()
{
    // TODO: Possible is better to move to separate business logic class
    for (auto connection : chronoconnections)
    {
        std::cout << "Checking limits for " << connection->ip() << std::endl;
        std::ostringstream query_oss;

        query_oss << "SELECT SUM(1) FROM online WHERE ip=" << ip_to_long(connection->ip()) << " ";
        std::cout << query_oss.str() << std::endl;
        auto dbresult = Db::query_select(query_oss.str());
        if (!dbresult)
        {
            std::cerr << "Mysql error: " << Db::mysql_error_msg << std::endl;
            break;
        }

        if (Db::num_fields(dbresult) > 0)
            while ( Db::row_type row = Db::fetch_row(dbresult) )
            {
                int minutes = 0;
                try
                {
                    minutes = std::stoi(row[0]);
                }
                catch (...)
                {
                    std::cerr << "Can't convert selected data to int: " << row[0] << std::endl;
                    break;
                }

                chronoscopist::chrmessage msg;
                if (minutes > MINUTES_LIMIT)
                    msg = chronoscopist::chrmessage::generate_message(chronoscopist::messagetype::lock, "Your time is out!");
                else
                    msg = chronoscopist::chrmessage::generate_message(chronoscopist::messagetype::unlock, "");

                connection->queue_tosend_push_message(msg);
                std::cout << "Total records: " << minutes << std::endl;
            }
    }
    

}

void ServerMsgManager::add_connection(ChronoServerConnection_ptr new_connection_ptr)
{
    ServerMsgManager::chronoconnections.emplace_back(new_connection_ptr);
}

std::list<ChronoServerConnection_ptr> ServerMsgManager::chronoconnections;

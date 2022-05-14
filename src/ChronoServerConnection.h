#pragma once
#include <memory>
#include <queue>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include "chronodto.h"

class CamProxy;
extern boost::asio::io_service service;

class ChronoServerConnection : public std::enable_shared_from_this<ChronoServerConnection>
{
    public:
        boost::asio::ip::tcp::socket & sock();
        ChronoServerConnection() : sock_(service) {};
        void start();
        void stop();
        void queue_tosend_push_message(chronoscopist::chrmessage);
        void queue_received_push_message(chronoscopist::chrmessage);
        chronoscopist::chrmessage queue_pop_message();

    protected:
        ChronoServerConnection(const ChronoServerConnection&) = delete;
        ChronoServerConnection& operator=(const ChronoServerConnection&) = delete;
        boost::asio::ip::tcp::socket sock_;

        void on_read(const boost::system::error_code & err, const size_t bytes);
        void on_write(const boost::system::error_code & err, const size_t bytes);
        void do_read();
        void send_messages();
        size_t read_complete(const boost::system::error_code & err, const size_t bytes);
        void do_connect();
        void do_reconnect();
        void do_disconnect();
        chronoscopist::chrmessage read_buffer_;
        std::string ip();
        std::queue<chronoscopist::chrmessage> messages_in;
        std::queue<chronoscopist::chrmessage> messages_out;
};

typedef std::shared_ptr<ChronoServerConnection> ChronoServerConnection_ptr;

#pragma once
#include <memory>
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
        chronoscopist::message read_buffer_;
        std::string ip();
};

typedef std::shared_ptr<ChronoServerConnection> ChronoServerConnection_ptr;

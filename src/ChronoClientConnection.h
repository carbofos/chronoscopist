#pragma once
#include <memory>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <string>
#include "chronodto.h"

using namespace boost::asio::ip;

class ChronoClientConnection : public std::enable_shared_from_this<ChronoClientConnection>
{
    public:
        ChronoClientConnection(boost::asio::io_service& io_service) : resolver_{io_service}, socket_{io_service} {};
        void connect(const std::string &ip, const std::string &port);
    private:
        tcp::resolver resolver_;
        tcp::socket socket_;
        boost::asio::streambuf request_;
        boost::asio::streambuf response_;
        chronoscopist::chrmessage write_buffer_;
        chronoscopist::chrmessage read_buffer_;

        // void do_connect();
        void closeconnection();
        // void closeconnection(const BaseCustomerNetworking_ptr &customers);
        void handle_resolve(const boost::system::error_code& err,
                tcp::resolver::iterator endpoint_iterator);
        void handle_connect(const boost::system::error_code& err,
                tcp::resolver::iterator endpoint_iterator);
        void handle_write_request(const boost::system::error_code& err);
        void do_read();
        void handle_read_some(const boost::system::error_code& err, std::size_t recvlen);
        void on_write(const boost::system::error_code & err, const size_t bytes);
        void on_read(const boost::system::error_code & err, const size_t bytes);

};

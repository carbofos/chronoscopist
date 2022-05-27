#include "ChronoClientConnection.h"
#include "ChronoServerConnection.h"
#include "config.h"

void ChronoClientConnection::connect(const std::string &ip, const std::string &port)
{
    std::cout << "connect()" << std::endl;
	boost::asio::ip::tcp::resolver::query query(ip, port);

	resolver_.async_resolve(query,
		boost::bind(&ChronoClientConnection::handle_resolve, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));

}

void ChronoClientConnection::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{
    std::cout << "handle_resolve()" << std::endl;
    if (!err)
    {
        tcp::endpoint endpoint = *endpoint_iterator;
        //   savedendpoint = endpoint;
        socket_.async_connect(endpoint,
                boost::bind(&ChronoClientConnection::handle_connect, shared_from_this(),
                boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cerr << err.message() << std::endl;
        closeconnection();
    }
}

void ChronoClientConnection::handle_connect(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{
    std::cout << "handle_connect()" << std::endl;
    if (!err)
    {
        message_tosend = chronoscopist::chrmessage::generate_message(chronoscopist::messagetype::none, "Hello");

        boost::asio::async_write(socket_, request_,
            boost::bind(&ChronoClientConnection::do_write, shared_from_this(),
              boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
        closeconnection();
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
            boost::bind(&ChronoClientConnection::handle_connect, shared_from_this(),
              boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        closeconnection();
    }
}

void ChronoClientConnection::do_write(const boost::system::error_code& err)
{
    std::cout << "do_write()" << std::endl;
    // do_read();
    
    socket_.async_write_some( boost::asio::buffer(&message_tosend, sizeof(message_tosend)),
            boost::bind(&ChronoClientConnection::on_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

    unused(err);
}

void ChronoClientConnection::on_write(const boost::system::error_code & err, const size_t bytes)
{
    if ( err)
    {
        std::cout << "Error in ChronoClientConnection::on_write" << std::endl;
        // stop();
        return;
    }

    std::cout << "ChronoClientConnection::on_write ok" << std::endl;
    do_read();
    unused(bytes);
}

void ChronoClientConnection::do_read()
{
    std::cout << "do_read()" << std::endl;
    boost::asio::async_read(socket_, boost::asio::buffer(&message_received, sizeof(chronoscopist::chrmessage)),
                boost::bind(&ChronoClientConnection::on_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
                );

    std::cout << "done do_read()" << std::endl;
}

void ChronoClientConnection::on_read(const boost::system::error_code & err, size_t bytes)
{
    if ( err) {
        std::cerr << "on_read() error\n";
        // stop();
        return;
        }
    if (bytes != sizeof(chronoscopist::chrmessage))
        std::cout << "Received bytes does not correspond size of chronoscopist::message\n";
    else
    {
        std::cout << "Received new message: " << message_received.text << std::endl;
        switch (message_received.type)
        {
        case chronoscopist::messagetype::ping:
            {
                std::cout << "PING command " << std::endl;
                boost::system::error_code err;
                message_tosend = chronoscopist::chrmessage::generate_message(chronoscopist::messagetype::pong, "Pong");
                do_write(err);
                break;
            }

        case chronoscopist::messagetype::lock:
            std::cout << "LOCK command " << std::endl;
            system("DISPLAY=:0 /usr/bin/xset dpms force off");
            break;

        case chronoscopist::messagetype::unlock:
            std::cout << "UNLOCK command " << std::endl;
            system("DISPLAY=:0 /usr/bin/xset dpms force on");
            break;

        default:
            std::cout << "Recevied unhandled msg type: " << message_received.type << std::endl;
            break;
        }
    }
    do_read();
}

void ChronoClientConnection::closeconnection()
{
    std::cout << "closeconnection()" << std::endl;
    boost::system::error_code ec;
    try
        {socket_.cancel();}
    catch (std::exception& e)
        { std::cout << e.what(); }

    try 
        {socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);}
    catch (std::exception& e)
        {std::cout << e.what(); }

    try 
        {socket_.close();}
    catch (std::exception& e)
        {std::cout << e.what(); }
	
}

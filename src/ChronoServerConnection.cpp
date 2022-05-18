#include <iostream>
#include "ChronoServerConnection.h"
#include "ServerMsgManager.h"
#include "config.h"

boost::asio::ip::tcp::socket & ChronoServerConnection::sock() { return sock_;}

void ChronoServerConnection::start()
{
    std::cout << "Starting new connection from " << ip() << std::endl;
    queue_tosend_push_message(chronoscopist::chrmessage::generate_message(chronoscopist::messagetype::ping, "Ping"));
    send_messages();
    do_read();
}

std::string ChronoServerConnection::ip()
{
    boost::system::error_code ec;
    return sock_.remote_endpoint(ec).address().to_string();
}

void ChronoServerConnection::stop()
{
  	boost::system::error_code ec;

    try //should be open to cancel
        {sock_.cancel();}
	catch (std::exception& e)
        {
            std::cout << "ChronoServerConnection::stop(): " << e.what() << std::endl;
        }
    try 
        {sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);}
	catch (std::exception& e)
        {
            std::cout << "ChronoServerConnection::stop(): " << e.what() << std::endl;
        }

    try 
        {sock_.close();}
	catch (std::exception& e)
        {
            std::cout << "ChronoServerConnection::stop(): " << e.what() << std::endl;
        }
   
    do_disconnect();
}

void ChronoServerConnection::on_read(const boost::system::error_code & err, size_t bytes)
{
    if ( err) {
        std::cout << "Error ChronoServerConnection::on_read: " << err.message() << std::endl;
        stop();
        return;
        }
    if (bytes != sizeof(chronoscopist::chrmessage))
        std::cout << "Received bytes does not correspond size of chronoscopist::message\n";
    else
    {
        // queue_received_push_message(read_buffer_);
        std::cout << "Received new message in queue " << messages_in.size() << ": " << read_buffer_.text << std::endl;
    }
    
    ServerMsgManager::process_received_message(read_buffer_, ip() );
    do_read();
}

void ChronoServerConnection::do_read()
{
    async_read(sock_, boost::asio::buffer(&read_buffer_, sizeof(chronoscopist::chrmessage)),
                boost::bind(&ChronoServerConnection::on_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
                );
}

void ChronoServerConnection::send_messages()
{
    while (messages_out.size() > 0)
    {
        auto outmsg = queue_pop_message();
        std::cout << "ChronoServerConnection::send_messages() with size " << sizeof(chronoscopist::chrmessage) << " to " << ip() << std::endl;
        // sock_.async_write_some( boost::asio::buffer(&outmsg, sizeof(chronoscopist::chrmessage)),
        //             boost::bind(&ChronoServerConnection::on_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );
        boost::asio::async_write(sock_, boost::asio::buffer(&outmsg, sizeof(chronoscopist::chrmessage)),
                boost::bind(&ChronoServerConnection::on_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );
    }

}

chronoscopist::chrmessage ChronoServerConnection::queue_pop_message()
{
        chronoscopist::chrmessage outmsg = messages_out.front();
        messages_out.pop();
        return outmsg;
}

void ChronoServerConnection::queue_tosend_push_message(chronoscopist::chrmessage newmessage)
{
    std::cout << "ChronoServerConnection::queue_tosend_push_message: " << newmessage.text << std::endl;
    messages_out.push(newmessage);
}

void ChronoServerConnection::queue_received_push_message(chronoscopist::chrmessage newmessage)
{
    messages_in.push(newmessage);
}

void ChronoServerConnection::on_write(const boost::system::error_code & err, const size_t bytes)
{
    std::cout << "Message on_write() send bytes " << bytes << std::endl;
    if ( err)
    {
        std::cerr << "Error in on_write(): " << err.message() << std::endl;
        return;
    }
}

size_t ChronoServerConnection::read_complete(const boost::system::error_code & err, const size_t bytes)
{
    if ( err)
    {
        // stop();
        return 0;
    }
    return 0;
    unused(bytes);
}

void ChronoServerConnection::do_reconnect()
{
    // auto cC = camClient.lock();
    // if ( cC != nullptr)
    // {
    //         try
    //         {
    //             cC->do_connect();
    //         }
    //         catch (std::logic_error &e)
    //         {
    //             BOOST_LOG_TRIVIAL(error) << "do_connect exception: " << e.what()  << std::endl;
    //         }
    // }
            
    // else
    //         throw std::runtime_error("camClient is null");
}

void ChronoServerConnection::do_disconnect()
{
    // if ( true || is_connected() )
    {
            // cC->closeconnection(shared_from_this() );
    }
}

#ifndef CHAT_SERVER_HPP_
#define CHAT_SERVER_HPP_

#include <boost/asio.hpp>
#include "chat_message.hpp"
#include "chat_room.hpp"

using boost::asio::ip::tcp;

class chat_server
{
public:
    chat_server(boost::asio::io_service &io_service,
            const tcp::endpoint &endpoint);

private:
    void do_accept();
    
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    chat_room m_room;
};

#endif

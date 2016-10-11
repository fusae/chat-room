#ifndef CHAT_CLIENT_HPP_
#define CHAT_CLIENT_HPP_

#include <boost/asio.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_deque;

class chat_client
{
public:
    chat_client(boost::asio::io_service &io_service,
            tcp::resolver::iterator endpoint_iterator);

    void write(const chat_message &msg);
    void close();

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator);
    void do_read_header();
    void do_read_body();
    void do_write();

    boost::asio::io_service &m_io_service;
    tcp::socket m_socket;
    chat_message m_read_msg;
    chat_message_deque m_write_msgs;
};

#endif

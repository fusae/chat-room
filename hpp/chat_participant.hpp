#ifndef CHAT_PARTICIPANT_HPP
#define CHAT_PARTICIPANT_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <deque>
#include <memory>
#include "chat_message.hpp"
#include "chat_room.hpp"

// -------------------------------------------
// for write messages queue
typedef std::deque<chat_message> chat_message_queue;
// -------------------------------------------

using boost::asio::ip::tcp;

class chat_room; // forward declaration
class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message &msg) = 0;
};

class chat_session: public chat_participant,
                    public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room &room);
    void start();
    void deliver(const chat_message &msg);

private:
    void do_write();
    void do_read_header();
    void do_read_username_header();
    void do_read_username();
    void do_read_body();

    tcp::socket m_socket;
    chat_room &m_room;
    chat_message m_read_msg;
    chat_message_queue m_write_msgs;
};


#endif 

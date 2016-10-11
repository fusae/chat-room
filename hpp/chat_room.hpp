#ifndef CHAT_ROOM_HPP
#define CHAT_ROOM_HPP

#include <set>
#include <iostream>
#include "chat_message.hpp"
#include "chat_participant.hpp"
#include <memory>
#include <deque>

// -------------------------------------------
// for write messages queue
typedef std::deque<chat_message> chat_message_queue;
// -------------------------------------------

class chat_participant; // forward declaration
typedef std::shared_ptr<chat_participant> chat_participant_ptr;


class chat_room
{
public:
    void join(chat_participant_ptr participant);
    void leave(chat_participant_ptr participant);
    void deliver(const chat_message &msg, chat_participant_ptr speaker);

private:
    std::set<chat_participant_ptr> m_participants;
    enum {max_recent_msgs = 100};
    chat_message_queue m_recent_msgs;
};

#endif

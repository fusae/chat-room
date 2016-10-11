#include <boost/asio.hpp>
#include <iostream>
#include "../hpp/chat_room.hpp"

// when a new member join, he will receive all
// the new recent messages
void chat_room::join(chat_participant_ptr participant)
{
    m_participants.insert(participant);
    for (auto msg: m_recent_msgs)
    {
        participant->deliver(msg);
    }
}

void chat_room::leave(chat_participant_ptr participant)
{
    m_participants.erase(participant);
}


// send message to all participants in this room
void chat_room::deliver(const chat_message &msg, chat_participant_ptr speaker)
{
    m_recent_msgs.push_back(msg);
    while (m_recent_msgs.size() > max_recent_msgs)
        m_recent_msgs.pop_front();

    for (auto participant: m_participants)
    {
        if (participant != speaker)
            participant->deliver(msg);
    }
}

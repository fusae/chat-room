#include "../hpp/chat_participant.hpp"

using boost::asio::ip::tcp;


chat_session::chat_session(tcp::socket socket, chat_room &room)
    : m_socket(std::move(socket)),
      m_room(room)
{}

void chat_session::start()
{
    m_room.join(shared_from_this());
    do_read_header();
}

void chat_session::deliver(const chat_message &msg)
{
    bool write_in_progress = !m_write_msgs.empty();
    m_write_msgs.push_back(msg);
    if (!write_in_progress)
    {
        do_write();
    }
}

void chat_session::do_write()
{
    auto self(shared_from_this()); // ?
    boost::asio::async_write(m_socket,
            boost::asio::buffer(m_write_msgs.front().data(), m_write_msgs.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    m_write_msgs.pop_front();
                    if (!m_write_msgs.empty())
                    {
                        do_write();
                    }
                }
                else
                {
                    m_room.leave(shared_from_this());
                }
            });
}

void chat_session::do_read_header()
{
    auto self(shared_from_this()); // ?
    boost::asio::async_read(m_socket,
            boost::asio::buffer(m_read_msg.data(),
                chat_message::header_length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && m_read_msg.decode_header())
                {
                    do_read_body();
                }
                else
                {
                    m_room.leave(shared_from_this());
                }
            });
}

void chat_session::do_read_body()
{
    auto self(shared_from_this()); // ?
    boost::asio::async_read(m_socket,
            boost::asio::buffer(m_read_msg.body(),
                m_read_msg.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    m_room.deliver(m_read_msg, self); // which client's message
                    do_read_header();
                }
                else
                {
                    m_room.leave(shared_from_this());
                }
            });
}

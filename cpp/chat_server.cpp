#include <iostream>
#include <boost/asio.hpp>
#include "../hpp/chat_message.hpp"
#include "../hpp/chat_server.hpp"
#include "../hpp/chat_room.hpp"
#include "../hpp/chat_participant.hpp"


chat_server::chat_server(boost::asio::io_service &io_service,
        const tcp::endpoint &endpoint)
    : m_acceptor(io_service, endpoint),
      m_socket(io_service)
{
    do_accept();
}

void chat_server::do_accept()
{
    m_acceptor.async_accept(m_socket,
            [this](boost::system::error_code ec)
            {
                if (!ec)
                {
                    std::make_shared<chat_session>(std::move(m_socket), m_room)->start();
                }

                do_accept();
            });
}

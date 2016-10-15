#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "../hpp/chat_message.hpp"
#include "../hpp/chat_client.hpp"

using boost::asio::ip::tcp;

chat_client::chat_client(boost::asio::io_service &io_service,
        tcp::resolver::iterator endpoint_iterator)
    : m_io_service(io_service), m_socket(io_service)
{
    do_connect(endpoint_iterator);
}

void chat_client::write(const chat_message &msg)
{
    // add handler
    m_io_service.post(
            [this, msg]
            {
                // do next write operation if the msg queue is empty
                bool write_in_progress = !m_write_msgs.empty();
                // now the queue is not empty
                m_write_msgs.push_back(msg);
                if (!write_in_progress)
                {
                    do_write();
                }
            });
}

void chat_client::close()
{
    m_io_service.post([this]() {m_socket.close();});
}

void chat_client::do_connect(tcp::resolver::iterator endpoint_iterator)
{
    // call async_connect
    boost::asio::async_connect(m_socket, endpoint_iterator, [this](boost::system::error_code ec, tcp::resolver::iterator)
            {
                if (!ec)
                {
                    do_read_header();
                }
            });
}

void chat_client::do_read_header()
{
    // call async_read
    boost::asio::async_read(m_socket,
            boost::asio::buffer(m_read_msg.data(), chat_message::header_length),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && m_read_msg.decode_header())
                {
                    do_read_username_header();
                }
                else
                {
                    // error
                    m_socket.close();
                }
            });
}

void chat_client::do_read_username_header()
{
    // call async_read
    boost::asio::async_read(m_socket,
            boost::asio::buffer(m_read_msg.data()+chat_message::header_length, chat_message::username_header_length),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && m_read_msg.decode_username_header())
                {
                    do_read_username();
                }
                else
                {
                    // error
                    m_socket.close();
                }
            });
}

void chat_client::do_read_username()
{
    // call async_read
    boost::asio::async_read(m_socket,
            boost::asio::buffer(m_read_msg.username(), m_read_msg.username_length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read_body();
                }
                else
                {
                    // error
                    m_socket.close();
                }
            });
}

void chat_client::do_read_body()
{
    // call async_read
    boost::asio::async_read(m_socket,
            boost::asio::buffer(m_read_msg.body()-chat_message::max_username_length+m_read_msg.username_length(), chat_message::max_username_length-m_read_msg.username_length()+m_read_msg.body_length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    std::cout << "\n";
                    std::cout.write(m_read_msg.username(), m_read_msg.username_length());
                    std::cout << ": ";
                    std::cout.write(m_read_msg.body(), m_read_msg.body_length());
                    std::cout << "\n";
                    do_read_header();
                }
                else
                {
                    // error
                    m_socket.close();
                }
            });
}

void chat_client::do_write()
{
    // call async_write
    boost::asio::async_write(m_socket,
            boost::asio::buffer(m_write_msgs.front().data(), m_write_msgs.front().length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
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
                    // error
                    m_socket.close();
                }
            });
}


int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: my_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({argv[1], argv[2]});
        chat_client c(io_service, endpoint_iterator);

        std::thread t([&io_service](){io_service.run();});

        char username[chat_message::max_username_length + 1];
        std::cout << "Please input the username: ";
        std::cin.getline(username, chat_message::max_username_length + 1);
        std::cout << username << ": ";

        char line[chat_message::max_body_length + 1];
        while (std::cin.getline(line, chat_message::max_body_length + 1))
        {
            chat_message msg;
            msg.body_length(std::strlen(line));
            msg.encode_header();
            msg.username_length(std::strlen(username));
            msg.encode_username_header();
            std::memcpy(msg.username(), username, msg.username_length());

            std::memcpy(msg.body(), line, msg.body_length());

            std::cout.write(msg.username(), msg.username_length());
            std::cout << ": ";

            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";    
    }

    return 0;
}

#include <iostream>
#include <boost/asio.hpp>
#include "../hpp/chat_server.hpp"
#include <list>

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: my_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; i++)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            // call chat_server constructor
            servers.emplace_back(io_service, endpoint);
        }

        io_service.run();
    }
    catch(std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

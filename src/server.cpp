#include "server.h"
#include "connectionhandler.h"
#include "urimapping.h"

namespace Devox
{
    
Server::Server(uint16_t port)
: m_acceptor(m_IOContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
, m_uriMapping(std::make_shared<UriMapping>())
{
    std::cout << "[http-server] Server is listening on port " << port << " ..." << std::endl;

    WaitForNewConnection();
    m_IOContext.run();
}

Server::~Server()
{
    m_IOContext.stop();
}

void Server::WaitForNewConnection()
{
    m_acceptor.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "[http-server] new connection: " 
                          << socket.remote_endpoint() << std::endl;
                auto connection = ConnectionHandler::Create(std::move(socket), m_uriMapping);
                connection->Start();
            }
            else
            {
                std::cerr << "[http-server] connection error: " << ec.message() << std::endl;
            }

            WaitForNewConnection();
        });
}
//namespace
}
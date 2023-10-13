#include "server.h"

namespace Devox
{
    
Server::Server(uint16_t port)
: m_acceptor(m_IOContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
    std::cout << "[http-server] Server listening on port " << port << " ..." << std::endl;

    WaitForNewConnection();
    // m_thread = std::thread([this]() { m_IOContext.run(); });
    m_IOContext.run();
}

Server::~Server()
{
    m_IOContext.stop();
    
    // if(m_thread.joinable()) m_thread.join();
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
                auto connection = ConnectionHandler::Create(m_IOContext, std::move(socket));
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
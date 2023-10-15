#ifndef SERVER
#define SERVER

#include "common.hpp"

namespace Devox
{

class UriMapping;

class Server
{
public:
    Server(uint16_t port);
    ~Server();

private:
    void WaitForNewConnection();
private:
    asio::io_context m_IOContext;
    asio::ip::tcp::acceptor m_acceptor;
    std::thread m_thread;

    std::shared_ptr<UriMapping> m_uriMapping;
};

}
#endif
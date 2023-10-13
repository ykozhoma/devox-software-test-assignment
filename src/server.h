#include "connectionhandler.h"

namespace Devox
{

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
};

}
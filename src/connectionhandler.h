#ifndef CONNECTION_HANDLER
#define CONNECTION_HANDLER

#include "common.hpp"
#include "safequeue.h"
#include "httpparser.h"

class ConnectionHandler : public std::enable_shared_from_this<ConnectionHandler>
{
public:
    static std::shared_ptr<ConnectionHandler> Create(asio::io_context& ctx,
                                                    asio::ip::tcp::socket socket)
    {
        return std::make_shared<ConnectionHandler>(ctx, std::move(socket));
    }

    ConnectionHandler(asio::io_context& ctx, asio::ip::tcp::socket socket)
    : m_ctx(ctx), m_socket(std::move(socket)), m_buffer(1024 * 20) { }

    const asio::ip::tcp::socket& socket() const
    {
        return m_socket;
    }

public:
    void Start()
    {
        m_socket.async_read_some(asio::buffer(m_buffer.data(), m_buffer.size()),
        std::bind(&ConnectionHandler::HandleRead
                , shared_from_this()
                , std::placeholders::_1
                , std::placeholders::_2));
    }

private:
    void HandleRead(std::error_code ec, size_t bytes)
    {
        if (!ec)
        {
            LogClientMessage();

            if (m_httpParser.ParseRequest(m_buffer.data()))
            {
                auto response = m_httpParser.GenerateResponse();

                m_responseQueue.push_back(std::move(response));

                m_socket.async_write_some(
                    asio::buffer(response.data(), response.size()),
                    std::bind(&ConnectionHandler::HandleWrite,
                    shared_from_this(),
                    std::placeholders::_1, std::placeholders::_2));
            }
            else
            {
                std::cerr << "[http-server]" << " request failed!" << std::endl;
                m_socket.close();
            }
        }
        else
        {
            std::cerr << "[http-server] error: " << ec.message() << std::endl;
            m_socket.close();
        }
    }

    void HandleWrite(std::error_code ec, size_t bytes)
    {
        if (!ec)
        {
            std::cout << "[http-server] Server has sent response..." << std::endl;
            auto response = m_responseQueue.pop_front();

            for (auto r : response)
                std::cout << r;
            
            m_socket.close();
            std::cout << "\n[http-server] Connection done OK!" << "\n\n";
        }
        else
        {
            m_socket.close();
            std::cerr << "error: " << ec.message() << std::endl;
        }
    }

    void LogClientMessage()
    {
        for (const auto& line : m_buffer)
            std::cout << line;

        std::cout << "\n";
    }

private:
    asio::io_context& m_ctx;
	asio::ip::tcp::socket m_socket;
    std::vector<char> m_buffer;

    Devox::safe_queue m_responseQueue;
    Devox::HttpParser m_httpParser;

    std::string message = "Hello From Server!";
};

#endif
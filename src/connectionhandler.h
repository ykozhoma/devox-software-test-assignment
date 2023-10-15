#ifndef CONNECTION_HANDLER
#define CONNECTION_HANDLER

#include "common.hpp"
#include "httpparser.h"

namespace Devox
{

class UriMapping;

class ConnectionHandler : public std::enable_shared_from_this<ConnectionHandler>
{
public:
    static std::shared_ptr<ConnectionHandler> Create(asio::ip::tcp::socket socket, std::shared_ptr<UriMapping> mapping)
    {
        return std::make_shared<ConnectionHandler>(std::move(socket), mapping);
    }

    ConnectionHandler(asio::ip::tcp::socket socket, std::shared_ptr<UriMapping> mapping)
    : m_socket(std::move(socket)), m_buffer(1024 * 20), m_httpParser(mapping) { }

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

            if (m_httpParser.ParseRequestAndGenerateResponse(m_buffer.data()))
            {
                m_response = m_httpParser.GetResponse();

                m_socket.async_write_some(
                    asio::buffer(m_response.c_str(), m_response.length()),
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
            std::cout << m_response << std::endl;

            m_socket.close();
            std::cout << "[http-server] Connection done OK!" << "\n\n";
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
	asio::ip::tcp::socket m_socket;
    std::vector<char> m_buffer;

    HttpParser m_httpParser;
    std::string m_response;
};
//namespace
}

#endif
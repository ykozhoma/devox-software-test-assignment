#include "common.hpp"

namespace
{
    enum class HttpMethod
    {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATCH,
        NONE
    };

    std::string to_string(HttpMethod method)
    {
        switch (method)
        {
        case HttpMethod::GET:
            return "GET";
        case HttpMethod::HEAD:
            return "HEAD";
        case HttpMethod::POST:
            return "POST";
        case HttpMethod::PUT:
            return "PUT";
        case HttpMethod::DELETE:
            return "DELETE";
        case HttpMethod::TRACE:
            return "TRACE";
        case HttpMethod::OPTIONS:
            return "OPTIONS";
        case HttpMethod::CONNECT:
            return "CONNECT";
        case HttpMethod::PATCH:
            return "PATCH";
        }
    }

    HttpMethod method_from_string(const std::string &method) noexcept
    {
        if (method == to_string(HttpMethod::GET))
        {
            return HttpMethod::GET;
        }
        else if (method == to_string(HttpMethod::HEAD))
        {
            return HttpMethod::HEAD;
        }
        else if (method == to_string(HttpMethod::POST))
        {
            return HttpMethod::POST;
        }
        else if (method == to_string(HttpMethod::PUT))
        {
            return HttpMethod::PUT;
        }
        else if (method == to_string(HttpMethod::DELETE))
        {
            return HttpMethod::DELETE;
        }
        else if (method == to_string(HttpMethod::TRACE))
        {
            return HttpMethod::TRACE;
        }
        else if (method == to_string(HttpMethod::OPTIONS))
        {
            return HttpMethod::OPTIONS;
        }
        else if (method == to_string(HttpMethod::CONNECT))
        {
            return HttpMethod::CONNECT;
        }
        else if (method == to_string(HttpMethod::PATCH))
        {
            return HttpMethod::PATCH;
        }

        return HttpMethod::NONE;
    }
}

namespace Devox
{

    class HttpParser
    {
        using HttpMessage = std::vector<char>;

        struct HttpRequest
        {
            std::string method;
            std::string path;
            std::string http_version;
            std::string host;
            std::string user_agent;
            std::string accept;
            uint32_t content_length;
            std::string content_type;
        };

        public:
        HttpParser() {}
        ~HttpParser() {}
        HttpParser(const HttpParser& rhs) = delete;

        public:
        bool ParseRequest(const char* request) 
        {
            std::istringstream req(request);
            std::string line;
            bool isFirstLine = true;

            while(std::getline(req, line))
            {
                if (line.empty()) return false;

                std::istringstream lineStream(line);
                std::string headerName, headerValue;

                if (isFirstLine)
                {
                    std::getline(lineStream, headerValue);

                    std::istringstream requestLine(headerValue);
                    requestLine >> m_request.method >> m_request.path >> m_request.http_version;

                    isFirstLine = false;
                    continue;
                }
                else
                {
                    std::getline(lineStream, headerName, ':');
                }

                std::getline(lineStream, headerValue);

                // std::cout << headerName << std::endl;

                if (headerName == "Host")
                {
                    m_request.host = headerValue;
                }
                else if (headerName == "User-Agent")
                {
                    m_request.user_agent = headerValue;
                }
                else if (headerName == "Accept")
                {
                    m_request.accept = headerValue;
                }
                else if (headerName == "Content-Length")
                {
                    m_request.content_length = std::stoi(headerValue);
                }
                else if (headerName == "Content-Type")
                {
                    m_request.content_type = headerValue;
                }
                else
                {
                    m_body = headerName;
                }
            }

            // LogResponse();

            return true;
        }

        HttpMessage GenerateResponse()
        {   
            std::string responseBody;

            if (!m_request.content_length)
            {
                responseBody  = "HTTP/1.1 404 Not Found\r\n";
            }
            else
            {
                responseBody += "HTTP/1.1 200 OK\r\n";
                responseBody += "Content-Type: " + m_request.content_type + "\r\n";
                responseBody += "Content-Length: " 
                    + std::to_string(m_request.content_length) + "\r\n";
                responseBody += "\r\n";
                responseBody += m_body + "\r\n";
            }

            HttpMessage response;
            // response.assign(ss.str().begin(), ss.str().end());
            response.assign(responseBody.begin(), responseBody.end());
            return response;
        }

        private:
        void LogResponse()
        {
            std::cout << "Method: " << m_request.method << std::endl;
            std::cout << "Path: " << m_request.path << std::endl;
            std::cout << "HTTP Version: " << m_request.http_version << std::endl;
            std::cout << "Host: " << m_request.host << std::endl;
            std::cout << "User-Agent: " << m_request.user_agent << std::endl;
            std::cout << "Accept: " << m_request.accept << std::endl;
            std::cout << "Content-Length: " << m_request.content_length << std::endl;
            std::cout << "Content-Type: " << m_request.content_type << std::endl;
            std::cout << "Request Body: " << m_body << std::endl;
        }

        private:
        HttpRequest m_request;
        std::string m_body;
    };
}
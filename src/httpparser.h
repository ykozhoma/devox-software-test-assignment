#ifndef HTTP_PARSER
#define HTTP_PARSER

#include "common.hpp"
#include "httpheader.hpp"
#include "urimapping.h"

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

        return "NONE";
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
    using HttpMessage = std::vector<char>;

    class HttpParser
    {
        public:
        HttpParser() = default;
        HttpParser(std::shared_ptr<UriMapping> mapping) : m_uriMapping(mapping)
        {}
        ~HttpParser() {}
        HttpParser(const HttpParser& rhs) = delete;

        public:

        const std::string& GetResponse() const { return m_responseString; }

        bool ParseRequestAndGenerateResponse(const char* request)
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
                    m_request.body = headerName;
                }
            }

            // LogRequest(m_request);

            HttpMethod method = method_from_string(m_request.method);

            if(method == HttpMethod::POST)
            {
                m_uriMapping->PostEntry(m_request);
                m_responseString = GenerateResponse(200, m_request);
            }
            else if (method == HttpMethod::GET)
            {
                HttpHeader result;
                if (m_uriMapping->GetEntry(m_request.path, result))
                {
                    m_responseString = GenerateResponse(200, result);
                }
                else
                {
                    m_responseString = GenerateResponse(404, result);
                }
            }
            else if (method == HttpMethod::DELETE)
            {
                if(m_uriMapping->DeleteEntry(m_request.path))
                {
                    m_responseString = GenerateResponse(200, HttpHeader());
                }
                else
                {
                    m_responseString = GenerateResponse(404, HttpHeader());
                }
            }
            else
            {
                m_responseString = GenerateResponse(404, HttpHeader());
            }

            return true;
        }

        private:

        std::string GenerateResponse(size_t responseCode, const HttpHeader& requstBody)
        {   
            std::string responseString;

            if (responseCode == 404)
            {
                responseString  = "HTTP/1.1 404 Not Found\r\n";
            }
            else
            {
                responseString += "HTTP/1.1 200 OK\r\n";

                if (!requstBody.content_type.empty())
                    responseString += "Content-Type: " + requstBody.content_type + "\r\n";

                if (requstBody.content_length != 0u)
                    responseString += "Content-Length: "
                        + std::to_string(requstBody.content_length) + "\r\n";

                if (!requstBody.body.empty())
                {
                    responseString += "\r\n";
                    responseString += requstBody.body + "\r\n";
                }
            }

            return std::move(responseString);
        }

        private:
        void LogRequest(const HttpHeader& request)
        {
            std::cout << "Method: " << request.method << std::endl;
            std::cout << "Path: " << request.path << std::endl;
            std::cout << "HTTP Version: " << request.http_version << std::endl;
            std::cout << "Host: " << request.host << std::endl;
            std::cout << "User-Agent: " << request.user_agent << std::endl;
            std::cout << "Accept: " << request.accept << std::endl;
            std::cout << "Content-Length: " << request.content_length << std::endl;
            std::cout << "Content-Type: " << request.content_type << std::endl;
            std::cout << "Request Body: " << request.body << std::endl;
        }

        private:
        HttpHeader  m_request;
        std::string m_responseString;

        std::shared_ptr<UriMapping>    m_uriMapping;
    };
//namespace
}
#endif
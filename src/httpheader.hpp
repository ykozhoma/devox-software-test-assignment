#ifndef HTTP_HEADER
#define HTTP_HEADER
#include <string>
namespace Devox
{
    struct HttpHeader
    {
        HttpHeader()
        : method("")
        , path("")
        , http_version("")
        , host("")
        , user_agent("")
        , accept("")
        , content_type("")
        , content_length(0u)
        , body("")
        {}

        std::string method;
        std::string path;
        std::string http_version;
        std::string host;
        std::string user_agent;
        std::string accept;
        std::string content_type;
        uint32_t content_length;
        std::string body;
    };
//namespace
}
#endif
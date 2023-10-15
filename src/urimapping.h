#ifndef URI_MAPPING
#define URI_MAPPING

#include <map>
#include <mutex>

#include "httpheader.hpp"

namespace Devox
{
    class UriMapping
    {
        public:

        UriMapping() {}
        ~UriMapping() {}

        void PostEntry(const HttpHeader& entry)
        {
            std::lock_guard<std::mutex> l(m_mutex);
            m_uriMap[entry.path] = entry;
        }

        bool GetEntry(const std::string& key, HttpHeader& result)
        {
            std::lock_guard<std::mutex> l(m_mutex);
            auto it = m_uriMap.find(key);

            if (it != m_uriMap.end())
            {
                result = it->second;
                return true;
            }

            return false;
        }

        bool DeleteEntry(const std::string& key)
        {
            std::lock_guard<std::mutex> l(m_mutex);
            auto it = m_uriMap.find(key);

            if (it != m_uriMap.end())
            {
                m_uriMap.erase(it);
                return true;
            }

            return false;
        }

        private:
        std::map<std::string, HttpHeader> m_uriMap;
        std::mutex m_mutex;
    };
//namespace
}

#endif
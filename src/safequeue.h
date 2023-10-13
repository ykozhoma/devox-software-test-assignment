#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <deque>
#include <mutex>
#include <vector>
#include <condition_variable>

namespace Devox
{
    class safe_queue
    {
        using Message = std::vector<char>;

        public:
        safe_queue() = default;
        safe_queue(const safe_queue& rhs) = delete;
        ~safe_queue() { clear(); }

        public:
        const Message& front()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            return m_queue.front();
        }

        const Message& back()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            return m_queue.back();
        }

        Message pop_front()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            auto m = std::move(m_queue.front());
            m_queue.pop_front();
            return m;
        }

        Message pop_back()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            auto m = std::move(m_queue.back());
            m_queue.pop_back();
            return m;
        }

        void push_front(const Message& m)
        {
            {
                std::lock_guard<std::mutex> l(m_queueMutex);
                m_queue.emplace_front(std::move(m));
            }

            m_cvWait.notify_one();
        }

        void push_back(const Message& m)
        {
            {
                std::lock_guard<std::mutex> l(m_queueMutex);
                m_queue.emplace_back(std::move(m));
            }

            m_cvWait.notify_one();
        }

        void wait()
        {
            while (empty())
            {
                std::unique_lock<std::mutex> lk(m_waitMutex);
                m_cvWait.wait(lk, [this] { return empty(); });
            }
        }

        void clear()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            m_queue.clear();
        }

        bool empty()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            return m_queue.empty();
        }

        size_t size()
        {
            std::lock_guard<std::mutex> l(m_queueMutex);
            return m_queue.size();
        }

        private:
        std::deque<Message> m_queue;

        std::mutex m_queueMutex;

        std::mutex m_waitMutex;
        std::condition_variable m_cvWait;
    };
}
#endif
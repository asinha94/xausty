#ifndef XSTY_EPOLL_H
#define XSTY_EPOLL_H

#include <cstdint>
#include <stdexcept>
#include <sys/epoll.h>

namespace xausty 
{
    class EpollException : public std::runtime_error
    {
    public:
        EpollException(const char* what) : std::runtime_error(what) {}
    };

    class Epoll {
    public:
        using EpollEvent = struct epoll_event;
        using EpollEventPtr = struct epoll_event *;
        Epoll();
        ~Epoll();
        bool pollOnAccepts(int fd);
        bool pollOnClients(int fd);
        //bool stop_watching(int fd);
        int wait();
        EpollEventPtr getEvents() {return m_events;}
    private:
        int m_fd;
        const int32_t m_max_events;
        EpollEvent m_ev;
        EpollEventPtr m_events;
    };

}

#endif
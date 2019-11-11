#ifndef XSTY_EPOLL_H
#define XSTY_EPOLL_H

#include <cstdint>
#include <string>

namespace xausty {

    class EpollException {
    public:
        EpollException(std::string msg) : m_msg(msg) {}
    private:
        std::string m_msg;
    };

    class Epoll {
    public:
        using EpollEvent = struct epoll_event;
        using EpollEventPtr = struct epoll_event *;
        Epoll();
        ~Epoll();
        bool start_listening_on_server(int fd);
        bool start_watching_client(int fd, bool readble, bool writeable);
        //bool stop_watching(int fd);
        int wait();
        EpollEventPtr get_events() {return m_events;}
    private:
        int m_fd;
        const int32_t m_max_events;
        EpollEvent m_ev;
        EpollEventPtr m_events;
    };

}

#endif
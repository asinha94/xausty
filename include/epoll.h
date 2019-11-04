#ifndef XSTY_EPOLL_H
#define XSTY_EPOLL_H

#include <cstdint>
#include <sys/epoll.h>

namespace xausty {

    class Epoll {
        using EpollEvent = struct epoll_event;
        using EpollEventPtr = struct epoll_event *;
    public:
        Epoll(int32_t fd, int32_t max_events);
        bool start_watching(int32_t fd, bool readble, bool writeable));
        bool stop_watching(int32_t fd);;
        int32_t wait();
    private:
        int32_t fd;
        const int32_t max_events;
        EpollEvent ev;
        EpollEventPtr events;
    };

}

#endif
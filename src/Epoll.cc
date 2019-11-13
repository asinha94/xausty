// std headers
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
// Linux headers
#include <sys/epoll.h>
// Project headers
#include <Epoll.h>


namespace xausty {

    using EpollEvent = Epoll::EpollEvent;
    using EpollEventPtr = Epoll::EpollEventPtr;

    Epoll::Epoll() :
        m_max_events(32)
    {
        // create epoll fd
        m_fd = epoll_create1(0);
        if (m_fd == -1) {
            std::string error = "Failed to create Epoll Descriptor";
            error += std::string(strerror(errno));
            throw EpollException(error.c_str());
        }

        // allocate memory for epoll buffer
        m_events = (EpollEventPtr) malloc(m_max_events * sizeof(m_ev));
        if (m_events == NULL) {
            throw EpollException("Failed to allocate buffer for epoll");
        }

    }

    Epoll::~Epoll()
    {
        free(m_events);
    }

    bool Epoll::pollOnAccepts(int fd) {
        m_ev.events = EPOLLIN;
        m_ev.data.fd = fd;
        int epollctl = epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &m_ev);
        if (epollctl == -1) {
            std::cout << "Couln't regsiter fd for epoll"
                << std::string(strerror(errno));
            return false;
        }
        return true;
    }

    bool Epoll::pollOnClients(int fd)
    {
        // Edge triggered notifications for RW
        m_ev.events = EPOLLET | EPOLLIN | EPOLLOUT;
        m_ev.data.fd = fd;
        int epollctl = epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &m_ev);
        if (epollctl == -1) {
            std::cout << "Couln't regsiter fd for epoll"
                << std::string(strerror(errno));
            return false;
        }
        return true;
    }

    int Epoll::wait()
    {
        std::cout << "wait()ing for epoll trigger\n";
        return epoll_wait(m_fd, m_events, m_max_events, -1);
    }

}
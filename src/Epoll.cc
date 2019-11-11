// C STD headers
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <cstdlib>
// C++ STD headers
#include <string>
#include <iostream>
// Linux-specific headers
#include <sys/epoll.h>
// Custom headers
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
            throw EpollException(error);
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

    bool Epoll::start_listening_on_server(int fd) {
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

    bool Epoll::start_watching_client(int fd, bool can_read, bool can_write)
    {
        uint32_t flags = EPOLLET;
        flags |= can_read ? EPOLLIN : (uint32_t) 0;
        flags |= can_write ? EPOLLOUT : (uint32_t) 0;

        m_ev.events = flags;
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
        std::cout << "Waiting for epoll data\n";
        return epoll_wait(m_fd, m_events, m_max_events, -1);
    }
}
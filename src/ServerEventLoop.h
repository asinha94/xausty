#ifndef XSTY_SERVER_EVT_LOOP_H
#define XSTY_SERVER_EVT_LOOP_H
// std headers
#include <cstdint>
// project headers
#include <Epoll.h>
#include <AsyncSocket.h>

namespace xausty {

    class ServerEventLoop
    {
    public:
        using EpollEvent = Epoll::EpollEvent;
        ServerEventLoop(uint16_t port, int listen_queue_size);
        void run();
        void handle_new_connection(const EpollEvent& event);
        void handle_client_read(const EpollEvent& event);
        void handle_client_error(const EpollEvent& event);
        void handle_client_close(const EpollEvent& event);
    private:
        Epoll m_epoll_handler;
        AsyncSocket m_server_socket;
        int m_listen_queue_size;
    };
}

#endif
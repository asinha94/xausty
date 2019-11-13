
// std headers
#include <cstdint>
// project headers
#include <ServerEventLoop.h>
#include <Epoll.h>
#include <AsyncSocket.h>

namespace xausty {

    using EpollEvent = ServerEventLoop::EpollEvent;

    ServerEventLoop::ServerEventLoop(uint16_t port, int listen_queue_size) :
        m_epoll_handler(Epoll()),
        m_server_socket(AsyncSocket(port)),
        m_listen_queue_size(listen_queue_size)
    {
        // create socket binding
        m_server_socket.bindAndListen(this->m_listen_queue_size);

    }

    void run();
    void handle_new_connection(const EpollEvent& event);
    void handle_client_read(const EpollEvent& event);
    void handle_client_error(const EpollEvent& event);
    void handle_client_close(const EpollEvent& event);
}

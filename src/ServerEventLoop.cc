
// std headers
#include <cstdint>
#include <iostream>
// linux headers
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
// project headers
#include <ServerEventLoop.h>
#include <Epoll.h>
#include <AsyncSocket.h>

const ssize_t BUF_LEN = 1024;

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

    void ServerEventLoop::run()
    {
        // start accepting connections on the socket
        int server_fd = this->m_server_socket.getfd();
        this->m_epoll_handler.pollOnAccepts(server_fd);
        std::cout << "Listening for incoming connections on "
                  << this->m_server_socket.getaddrinfo()
                  << '\n';

        // start the event loop
        for (;;) {
            int rdy_fds = this->m_epoll_handler.wait();
            auto events = this->m_epoll_handler.getEvents();

            // start processing events
            for (int i = 0; i < rdy_fds; ++i) {

                // handle new connection
                if (events[i].data.fd == server_fd) {
                    while (true) {
                        // Accept connection clients
                        auto client = this->m_server_socket.accept();
                        if (!client) {
                            break;
                        }
                        auto conn_client = client.value();
                        this->m_epoll_handler.pollOnClients(conn_client.getfd());
                        std::cout << "New client at " << conn_client.getaddrinfo() << '\n';
                    }
                }

                // handle errors
                else if (events[i].events & (EPOLLERR | EPOLLRDHUP | EPOLLHUP)) {
                    printf("client socket shutdown\n");
                    close(events[i].data.fd);
                }

                // can read
                else if (events[i].events & EPOLLIN) {
                    // ready for reading....
                    char buffer[BUF_LEN];
                    ssize_t valread = read(events[i].data.fd, buffer, BUF_LEN - 1L);
                    
                    // TODO: check for 0 bytes to check for disconnect
                    
                    buffer[valread] = '\0';
                    printf("Recv: %s\n", buffer);
                    char const * s = "Thanks for coming!\n";
                    // for now use the blocking call...hopefully this works
                    ssize_t sendsize = send(events[i].data.fd, s, 18, 0);
                    if (sendsize == -1) {
                        perror("Failed to send data to client");
                    }
                }

            }


        }
    }


    //void ServerEventLoop::handle_new_connection(const EpollEvent& event) {}
    //void ServerEventLoop::handle_client_read(const EpollEvent& event) {}
    //void ServerEventLoop::handle_client_error(const EpollEvent& event) {}
    //void ServerEventLoop::handle_client_close(const EpollEvent& event) {}
}

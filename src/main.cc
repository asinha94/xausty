// STD headers
#include <cstdio>
#include <iostream>
// linux headers
#include <unistd.h>
// project headers
#include <Epoll.h>
#include <AsyncSocket.h>

// to remove
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

const ssize_t BUF_LEN = 1024;

int main(void)
{
    // Create my epoll descriptor
    auto epoll_event_loop = xausty::Epoll();

    // create listening socket
    auto server = xausty::AsyncSocket();
    server.bindAndListen(8080, 10);
    int server_fd = server.m_fd;

    // register accept-socket for events
    epoll_event_loop.pollOnAccepts(server_fd);

    // Wait for events
    for ( ;; ) {
        int rdy_fds = epoll_event_loop.wait();
        auto events = epoll_event_loop.getEvents();

        for (int i = 0; i < rdy_fds; ++i) {
            // new client connection
            if (events[i].data.fd == server_fd) {
                // new socket connection
                while (true) {
                    
                    // Accept connection clients
                    auto client = server.accept();
                    if (!client) {
                        break;
                    }
                    auto conn_client = client.value();
                    epoll_event_loop.pollOnClients(conn_client.m_fd);
                    std::cout << "New Connected client!\n";
                }
            }

            // handler errors
            else if (events[i].events & (EPOLLERR | EPOLLRDHUP | EPOLLHUP)) {
                printf("client socket shutdown\n");
                close(events[i].data.fd);
            }

            // can read
            else if (events[i].events & EPOLLIN) {
                // ready for reading....
                char buffer[BUF_LEN];
                ssize_t valread = read(events[i].data.fd, buffer, BUF_LEN - 1L);
                // check for 0 bytes to check for disconnect
                
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

    return 0;
}
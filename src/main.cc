#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <Epoll.h>

#define PORT 8080
#define LISTENQ_SIZE 10
const ssize_t BUF_LEN = 1024;

int
main(void)
{
    // Create my epoll descriptor
    auto epoll_event_loop = xausty::Epoll();

    int server_fd = socket(AF_INET,
                           SOCK_STREAM | SOCK_NONBLOCK,
                           0);
    if (server_fd == -1) {
        perror("Failed to create socket()");
        return -1;
    }

    // enable Port and address re-use
    int optval = 1;
    int sockopt = setsockopt(server_fd,
                             SOL_SOCKET,
                             SO_REUSEADDR | SO_REUSEPORT,
                             &optval,
                             sizeof(optval));
    if (sockopt) {
        perror("Failed to set socket options");
        return -1;
    }

    // Bind to configured endpoint
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    int socklen = sizeof(address);

    int bindval = bind(server_fd,
                       (struct sockaddr *) &address,
                       socklen);
    if (bindval < 0) {
        perror("Couldn't bind to addr:port");
        return bindval;
    }

    int listenval = listen(server_fd, LISTENQ_SIZE);
    if (listenval < 0) {
        perror("Failed to listen() on socket");
        return -1;
    }

    // register accept-socket for events
    epoll_event_loop.start_listening_on_server(server_fd);

    // Wait for events
    for ( ;; ) {
        int rdy_fds = epoll_event_loop.wait();
        auto events = epoll_event_loop.get_events();

        for (int i = 0; i < rdy_fds; ++i) {
            // new client connection
            if (events[i].data.fd == server_fd) {
                // new socket connection
                printf("New client connection\n");
                int client_sock_fd;
                while ((client_sock_fd = accept4(server_fd,
                                             (struct sockaddr *) &address,
                                             (socklen_t *) &socklen,
                                             SOCK_NONBLOCK)) != -1) {
                    if (client_sock_fd < 0) {
                        perror("Couldn't accept() client socket connection");
                    }

                    epoll_event_loop.start_watching_client(client_sock_fd, true, false);
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
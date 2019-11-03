#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#define PORT 8080
#define LISTENQ_SIZE 10
const ssize_t BUF_LEN = 1024;

int
main(void)
{
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

    for ( ;; ) {
        printf("Waiting for clients....\n");
        int client_sock_fd = accept(server_fd,
                            (struct sockaddr *) &address,
                            (socklen_t *) &socklen);
        if (client_sock_fd < 0) {
            perror("Couldn't accept() client socket connection");
            return -1;
        }

        // client connected
        printf("Connected to client\n");
        char buffer[BUF_LEN];
        ssize_t valread = read(client_sock_fd, buffer, BUF_LEN - 1L);
        buffer[valread] = '\0';
        printf("Recv: %s\n", buffer);
        char const * s = "Thanks for coming!\n";
        send(client_sock_fd, s, 18, 0);
        printf("Replied!\n");
    }

    
    return 0;
}
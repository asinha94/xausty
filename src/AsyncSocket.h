#ifndef XSTY_SOCKET_H
#define XSTY_SOCKET_H
// std headers
#include <cstdint>
#include <string>
#include <experimental/optional>
#include <stdexcept>

namespace xausty
{
    class SocketException : public std::runtime_error
    {
    public:
        SocketException(const char* what) : std::runtime_error(what) {}
    };

    class AsyncSocket
    {
    public:
        using SockAddr = struct sockaddr_in;
        using SockAddrPtr = struct sockaddr_in *;
        
        AsyncSocket(uint16_t port);
        // syscall wrappers
        bool bindAndListen(int listenq_len);
        std::experimental::optional<AsyncSocket> accept();
        std::string read();
        bool write(const std::string &data);
        // other wrappers
        std::string getaddrinfo();
        int getfd() {return this->m_fd;}
        
    private:
        int m_fd;
        uint32_t m_addr;
        uint16_t m_port;
        bool m_server_side;
        AsyncSocket(int client_fd, uint32_t addr, uint16_t port);
    };
}

#endif
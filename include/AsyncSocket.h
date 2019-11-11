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
        int m_fd; // TODO: make private when refactored
        AsyncSocket();
        bool bindAndListen(const uint16_t port, int listenq_len);
        std::experimental::optional<AsyncSocket> accept();
        std::string read();
        bool write(const std::string &data);
        
    private:
        bool m_server_side;
        uint16_t m_port;
        AsyncSocket(int client_fd) : m_fd(client_fd), m_server_side(false) {}
    };
}

#endif
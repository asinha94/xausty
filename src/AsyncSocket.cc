// STD headers
#include <cerrno>
#include <cstring>
#include <string>
#include <experimental/optional>
// Linux headers
#include <sys/socket.h>
#include <netinet/in.h>
// Project headers
#include <AsyncSocket.h>

namespace xausty {

    using SockAddr = AsyncSocket::SockAddr;
    using SockAddrPtr = AsyncSocket::SockAddrPtr;

    AsyncSocket::AsyncSocket(uint16_t port) :
        m_addr(0),
        m_port(port),
        m_server_side(true)
    {
        this->m_fd = socket(AF_INET,
                            SOCK_STREAM | SOCK_NONBLOCK,
                            0);

        if (this->m_fd == -1) {
            std::string error = "Failed to create socket()";
            error += std::string(strerror(errno));
            throw SocketException(error.c_str());
        }

        // enable Port and address re-use
        int optval = 1;
        int sockopt = setsockopt(this->m_fd,
                                SOL_SOCKET,
                                SO_REUSEADDR | SO_REUSEPORT,
                                &optval,
                                sizeof(optval));
        if (sockopt) {
            std::string error = "Failed to set socket options";
            error += std::string(strerror(errno));
            throw SocketException(error.c_str());
        }
    }

    AsyncSocket::AsyncSocket(int client_fd, uint32_t addr, uint16_t port) :
        m_fd(client_fd),
        m_addr(addr),
        m_port(port),
        m_server_side(false)
    {

    }
  
    bool AsyncSocket::bindAndListen(int listenq_len)
    {
        if (!this->m_server_side) {
            return false;
        }

        // IPV4 only for now
        SockAddr address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(this->m_port);
        int socklen = sizeof(address);

        // bind socket to (interface, port)
        int bindval = bind(this->m_fd,
                           (struct sockaddr *) &address,
                           socklen);
        if (bindval < 0) {
           // TODO: log it probs
            return false;
        }

        int listenval = listen(this->m_fd, listenq_len);
        if (listenval < 0) {
            // log
            return false;
        }
        return true;

    }

    std::experimental::optional<AsyncSocket> AsyncSocket::accept()
    {
        // Accept connection if there is any
        SockAddr addr;
        int socklen = sizeof(addr);
        int client_sock_fd = accept4(this->m_fd,
                                     (struct sockaddr *) &addr,
                                     (socklen_t *) &socklen,
                                     SOCK_NONBLOCK);

        if (client_sock_fd == -1) {
            // log it
            return std::experimental::nullopt;
        }

        return AsyncSocket(client_sock_fd, addr.sin_addr.s_addr, addr.sin_port);

    }

    std::string AsyncSocket::read()
    {
        return "";
    }

    bool AsyncSocket::write(const std::string &data)
    {
        return data.length() == 1;
    }

    std::string AsyncSocket::getaddrinfo()
    {
        uint8_t o1 = (uint8_t) ((this->m_addr & 0xFF000000) >> 24);
        uint8_t o2 = (uint8_t) ((this->m_addr & 0x00FF0000) >> 16);
        uint8_t o3 = (uint8_t) ((this->m_addr & 0x0000FF00) >> 8);
        uint8_t o4 = (uint8_t) ((this->m_addr & 0x000000FF) >> 0);

        std::string addr = std::to_string(o4) + "." 
                         + std::to_string(o3) + "."
                         + std::to_string(o2) + "."
                         + std::to_string(o1) + ":"
                         + std::to_string(this->m_port);
        return addr;
    }

}
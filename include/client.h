#ifndef XSTY_CLIENT_H
#define XSTY_CLIENT_H

#include <cstdint>

namespace xsty {
    // Client inbound connections
    class ClientConnection {
    public:
        ClientConnection(int32_t fd);
        bool read(const char * data);
        bool write(const char * data);
    private:
        int32_t fd;
        bool can_read;
        bool can_write;
        int32_t state;  
    };

}
#endif
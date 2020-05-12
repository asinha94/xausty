// project headers
#include <ServerEventLoop.h>

const ssize_t BUF_LEN = 1024;

int main(void)
{
    // run event loop
    auto event_loop = xausty::ServerEventLoop(8080, 10);
    event_loop.run();
    return 0;
}
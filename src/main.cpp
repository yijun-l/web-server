#include "include/Server.h"

#define IP_ADDR "0.0.0.0"
#define PORT 8888

int main() {
    auto* server = new Server(IP_ADDR, PORT);
    server->run();
}

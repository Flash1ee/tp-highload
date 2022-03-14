#include <iostream>
#include "server.h++"

int main() {
    Server server(POOL_SIZE, PORT);
    if(!server.init()) {
        return 1;
    }
    std::cout << "The server is ON\n";
    server.run();

    return 0;
}

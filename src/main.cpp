#include "server/Server.h"
#include <stdio.h>
#include <stdint.h>

int main(void) {
    HttpServer server("127.0.0.1", 5000);
    server.start();
    return 0;
}

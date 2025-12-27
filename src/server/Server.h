#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

typedef struct sockaddr_in InetSocketAddress;
typedef struct sockaddr SocketAddress;

class HttpServer {
    private:
        int m_serverSocket;
        int m_clientSocket;
        socklen_t m_socketLength;
        InetSocketAddress m_serverInfo;
        InetSocketAddress m_clientInfo;
        uint16_t m_portNumber;
        std::string m_serverIp;
        char m_clientIp[INET_ADDRSTRLEN];
        std::string m_htmlContent;
        std::string m_htmlHeader;
        void readFileContent(std::string filename);
    public:
        HttpServer(std::string ipAddress, uint16_t portNumber);
        ~HttpServer();
        void start();
};

#include "Server.h"
#include "../parser/HttpParser.h"
#include <string.h>
#include <fstream>

HttpServer::HttpServer(std::string ipAddress, uint16_t portNumber) :
    m_serverIp(ipAddress), m_portNumber(portNumber) {
        m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        m_serverInfo.sin_family = AF_INET;
        inet_pton(AF_INET, m_serverIp.c_str(), &m_serverInfo.sin_addr.s_addr);
        m_serverInfo.sin_port = htons(m_portNumber);
        m_socketLength = sizeof(m_serverInfo);
        bind(m_serverSocket, (SocketAddress *) &m_serverInfo, m_socketLength);
}

HttpServer::~HttpServer() {
    close(m_serverSocket);
}

bool HttpServer::readFileContent(std::string filename) {
    std::string str;
    std::ifstream file("/home/ocirnexam/repos/webUI/build/html/" + filename);
    m_htmlContent = "";
    if(file.fail()) {
        return false;
    }
    while (std::getline(file, str))
    {
        m_htmlContent += str;
        m_htmlContent.push_back('\n');
    }  
    file.close();
    return true;
}

void HttpServer::start() {
    HttpParser parser;
    char readBuffer[1000];
    int readCount;
    std::string fullContentString;
    char *payload;

    if (m_serverSocket == -1) {
        std::cout << "Cannot listen for connection! Socket creation failed!" << std::endl;
        return;
    }
    if(listen(m_serverSocket, 1) == -1) {
        std::cout << "Failed to listen on port " << m_portNumber << std::endl;
        return;
    }
    while(1) {
        m_htmlContent = "";
        m_clientSocket = accept(m_serverSocket, (SocketAddress *)&m_clientInfo, &m_socketLength);
        if(m_clientSocket == -1) {
            std::cout << "Failed to create client socket!" << std::endl;
        }
        inet_ntop(AF_INET, &m_clientInfo.sin_addr.s_addr, m_clientIp, INET_ADDRSTRLEN);
        std::cout << "Client connected: " << m_clientIp << std::endl;
        
        // read request
        readCount = read(m_clientSocket, readBuffer, sizeof(readBuffer));
        readBuffer[readCount] = 0;
        //repeat reads until complete command received
        while(strstr(readBuffer, "\r\n\r\n") == NULL) {
            readCount += read(m_clientSocket, &readBuffer[readCount], sizeof(readBuffer) - readCount);
            readBuffer[readCount] = 0;
        }
        parser.parse(readBuffer);

        // request answer
        if(parser.getRequestedCommand() == HttpCommand::GET && 
           parser.getRequestedFilename() != "") {
            if(readFileContent(parser.getRequestedFilename())) {
                m_htmlHeader = "HTTP/1.1 200 OK\r\n\r\n";
            }
            else {
                m_htmlHeader = "HTTP/1.1 404 NOT_FOUND\r\n\r\n";
            }
        }
        else if (parser.getRequestedCommand() == HttpCommand::POST) {
            m_htmlHeader = "HTTP/1.1 200 OK\r\n\r\n";
            for (int i = 0; i < parser.getAmountOfArguments(); i++) {
                Argument a = parser.getArgument(i);
                std::cout << "Argument(" << a.name << ", " << a.value << ")" << std::endl;
                m_htmlContent += "<html><body><h1>" + a.name + " is now set to: " + a.value + "</h1></body></html>";
            }
        }
        else {
            m_htmlHeader = "HTTP/1.1 404 NOT_FOUND\r\n\r\n";
        }
        fullContentString = m_htmlHeader + m_htmlContent;
        payload = (char *) fullContentString.c_str();
        write(m_clientSocket, payload, strlen(payload));
        close(m_clientSocket);
    }
}

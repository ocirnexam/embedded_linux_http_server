#include "HttpParser.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define FILENAME_LENGTH_PADDING ((uint8_t) 6) // GET_/<filename>_HTTP From get to H = 6

void HttpParser::parse(char *httpPayload) {
    std::string payload = httpPayload;
    m_requestCommand = INVALID;
    m_requestedFile = "";
    int positionGet;
    int positionHttp;
    uint8_t filenameLength = 0;
    if ((positionGet  = payload.find("GET"))  != std::string::npos &&
        (positionHttp = payload.find("HTTP")) != std::string::npos) {
        m_requestCommand = GET;
        filenameLength = positionHttp - positionGet - FILENAME_LENGTH_PADDING;
        m_requestedFile = payload.substr(FILENAME_LENGTH_PADDING - 1, filenameLength);
        std::cout << "Request Command: " << m_requestCommand << "\nRequested File: " << m_requestedFile << std::endl;
    }
    
}

HttpCommand HttpParser::getRequestedCommand() {
    return m_requestCommand;
}

std::string HttpParser::getRequestedFilename() {
    return m_requestedFile;
}

uint8_t HttpParser::getAmountOfArguments() {
    return m_arguments.size(); 
}

Argument HttpParser::getArgument(uint8_t index) {
    if (index < m_arguments.size()) {
        return m_arguments[index];
    }
    return {"", ""};
}

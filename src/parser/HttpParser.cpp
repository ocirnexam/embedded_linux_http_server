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
    int positionStart;
    int positionEnd;
    std::string argumentName, argumentValue;
    uint8_t filenameLength = 0;
    if ((positionStart = payload.find("GET"))  != std::string::npos &&
        (positionEnd   = payload.find("HTTP")) != std::string::npos) {
        m_requestCommand = GET;
        filenameLength = positionEnd - positionStart - FILENAME_LENGTH_PADDING;
        m_requestedFile = payload.substr(FILENAME_LENGTH_PADDING - 1, filenameLength);
        if (m_requestedFile.empty()) {
            // means user requested /
            m_requestedFile = "index.html";
        }
    }
    else if ((positionStart = payload.find("POST")) != std::string::npos) {
        std::cout << payload << std::endl;
        m_requestCommand = POST;
        m_arguments.clear();
        if((positionStart = payload.find("\r\n\r\n")) != std::string::npos) {
            positionStart += 4; // need to skip newline characters
            positionEnd = payload.find("=", positionStart);
            argumentName = payload.substr(positionStart, positionEnd - positionStart);
            positionStart = positionEnd + 1;
            positionEnd = payload.find("\r\n", positionStart);
            argumentValue = payload.substr(positionStart, positionEnd - positionStart);
            m_arguments.push_back({.name = argumentName, .value = argumentValue});
        }
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

#pragma once

#include <string>
#include <vector>
#include <cstdint>

enum HttpCommand {
    GET,
    POST,
    PUT,
    DELETE,
    INVALID
};

struct Argument {
    std::string name;
    std::string value;
};

class HttpParser {
    private:
        HttpCommand m_requestCommand;
        std::string m_requestedFile;
        std::vector<Argument> m_arguments;
    public:
        HttpParser(){}
        ~HttpParser(){}
        void parse(char *httpPayload);
        HttpCommand getRequestedCommand();
        std::string getRequestedFilename();
        uint8_t getAmountOfArguments();
        Argument getArgument(uint8_t index);
};

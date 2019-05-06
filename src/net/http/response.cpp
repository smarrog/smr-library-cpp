#include "response.hpp"

#include <sstream>

using namespace smr;

HttpResponse& HttpResponse::readFrom(std::istream& is) {
    readCapture(is);
    readHeaders(is);
    readBody(is);
    return *this;
}

void HttpResponse::readCapture(std::istream& is) {
    if (is.peek() == -1) {
        throw std::runtime_error("No http capture");
    }
    std::string line;
    std::getline(is, line);
    std::istringstream iss(line);

    iss >> _version;
    int code;
    iss >> code;
    try {
        _code = static_cast<HTTP_CODE>(code);
    } catch (...) {
        throw std::runtime_error(std::string("Incorrect code -> ") + std::to_string(code));
    }
    iss >> _version;
    iss >> _description;
}

void HttpResponse::readHeaders(std::istream& is) {
    std::string line;
    std::getline(is, line);
    while (!line.empty()) {
        auto pos = line.find(':');
        if (pos == std::string::npos) {
            throw std::runtime_error(std::string("Wrong header line -> " + line));
        }
        _headers.emplace(line.substr(0, pos), line.substr(pos + 1));
        std::getline(is, line);
    }
}

void HttpResponse::readBody(std::istream& is) {
    auto it = _headers.find("Content-Length");
    if (it == _headers.end()) {
        return;
    }
    size_t contentSize = std::stoul(it->second);
    std::stringstream ss;
    for (auto i = 0; i < contentSize; i++) {
        auto c = is.get();
        if (c == -1) {
            throw std::runtime_error("Content-length header was " + std::to_string(contentSize) + " but read only " + std::to_string(i) + " until eof occurred");
        }
        ss << static_cast<char>(c);
    }
    _body = ss.str();
}
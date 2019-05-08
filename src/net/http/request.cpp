#include "request.hpp"

using namespace smr;

void HttpRequest::addHeader(std::string name, std::string body) {
    _headers[std::move(name)] = std::move(body);
}

std::string HttpRequest::serialize() const {
    std::ostringstream os;
    os << _method << ' ' << _uri << ' ' << _version <<"\r\n";
    for (const auto& [ headerName, headerBody ] : _headers) {
        os << headerName << ": " << headerBody << "\r\n";
    }
    os << "\r\n";
    return os.str();
}
#pragma once

#include <istream>
#include <unordered_map>

#include "codes.hpp"

namespace smr {
    class HttpResponse {
    public:
        HttpResponse& readFrom(std::istream& is);

        const std::string& getVersion() const {
            return _version;
        }

        HTTP_CODE getCode() const {
            return _code;
        }

        const std::string& getDescription() const {
            return _description;
        }

        const std::unordered_map<std::string, std::string>& getHeaders() const {
            return _headers;
        }

        const std::string& getBody() const {
            return _body;
        }

    private:
        void readCapture(std::istream& is);
        void readHeaders(std::istream& is);
        void readBody(std::istream& is);

        std::string _version;
        HTTP_CODE _code;
        std::string _description;
        std::unordered_map<std::string, std::string> _headers;
        std::string _body;
    };
}

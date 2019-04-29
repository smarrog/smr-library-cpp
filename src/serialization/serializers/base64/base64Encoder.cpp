#include "base64Encoder.hpp"
#include "serialization/exceptions.hpp"

using namespace smr;

const std::array<char, 64> CHARS = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

static inline bool isBase64(char c) {
    return std::isalnum(c) || c == '+' || c == '/';
}

void Base64Encoder::flush() {
    _buf4[0] = (_buf3[0] & 0xFC) >> 2;
    _buf4[1] = ((_buf3[0] & 0x03) << 4) + ((_buf3[1] & 0xF0) >> 4);
    _buf4[2] = ((_buf3[1] & 0x0F) << 2) + ((_buf3[2] & 0xC0) >> 6);
    _buf4[3] = _buf3[2] & 0x3F;

    for (auto i = 0; i <= _buf3Index; i++) {
        _os << CHARS[_buf4[i]];
    }
}

void Base64Encoder::encode(const Serializable& serializable) {
    if (!serializable.isString()) {
        throw std::runtime_error("value is not string");
    }
    for (auto c : serializable.asString()) {
        _buf3[_buf3Index++] = c;
        if (_buf3Index == 3) {
            flush();
            _buf3Index = 0;
        }
    }

    if (_buf3Index > 0) {
        for (auto i = _buf3Index; i < 3; i++) {
            _buf3[i] = '\0';
        }
        flush();
        while (_buf3Index++ < 3) {
            _os << '=';
        }
    }
}
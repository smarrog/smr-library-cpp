#include "base64.hpp"

#include <unordered_map>

using namespace smr;

const std::array<char, 64> CHARS = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

static std::unordered_map<char, char> CHAR_INDEXES {
    { 'A', 0 }, { 'B', 1 }, { 'C', 2 }, { 'D', 3 }, { 'E', 4 }, { 'F', 5 }, { 'G', 6 }, { 'H', 7 }, { 'I', 8 },
    { 'J', 9 }, { 'K', 10 }, { 'L', 11 }, { 'M', 12 }, { 'N', 13 }, { 'O', 14 }, { 'P', 15 }, { 'Q', 16 }, { 'R', 17 },
    { 'S', 18 }, { 'T', 19 }, { 'U', 20 }, { 'V', 21 }, { 'W', 22 }, { 'X', 23 }, { 'Y', 24 }, { 'Z', 25 },
    { 'a', 26 }, { 'b', 27 }, { 'c', 28 }, { 'd', 29 }, { 'e', 30 }, { 'f', 31 }, { 'g', 32 }, { 'h', 33 }, { 'i', 34 },
    { 'j', 35 }, { 'k', 36 }, { 'l', 37 }, { 'm', 38 }, { 'n', 39 }, { 'o', 40 }, { 'p', 41 }, { 'q', 42 }, { 'r', 43 },
    { 's', 44 }, { 't', 45 }, { 'u', 46 }, { 'v', 47 }, { 'w', 48 }, { 'x', 49 }, { 'y', 50 }, { 'z', 51 },
    { '0', 52 }, { '1', 53 }, { '2', 54 }, { '3', 55 }, { '4', 56 }, { '5', 57 }, { '6', 58 }, { '7', 59 }, { '8', 60 }, { '9', 61 },
    { '+', 62 }, { '/', 63 }
};

static inline bool isBase64(char c) {
    return std::isalnum(c) || c == '+' || c == '/';
}

std::string Base64::encode(const std::string& s) {
    Buf3 buf3;
    Buf4 buf4;
    std::ostringstream oss;
    size_t index = 0;

    for (auto c : s) {
        buf3[index++] = c;
        if (index == 3) {
            buf3ToBuf4(buf3, buf4, oss, index);
            index = 0;
        }
    }

    if (index > 0) {
        for (auto i = index; i < 3; i++) {
            buf3[i] = '\0';
        }
        buf3ToBuf4(buf3, buf4, oss, index);
        while (index++ < 3) {
            oss << '=';
        }
    }

    return oss.str();
}

std::string Base64::decode(const std::string& s) {
    Buf3 buf3;
    Buf4 buf4;
    std::ostringstream oss;
    size_t index = 0;

    for (auto c : s) {
        if (c == '=') {
            break;
        }
        if (!isBase64(c)) {
            throw std::runtime_error(std::string("Unexpected token: ") + c + " in base64");
        }
        buf4[index++] = c;
        if (index == 4) {
            buf4ToBuf3(buf3, buf4, oss, index);
            index = 0;
        }
    }
    if (index > 0) {
        buf4ToBuf3(buf3, buf4, oss, index);
    }
    return oss.str();
}

void Base64::buf3ToBuf4(Buf3& buf3, Buf4& buf4, std::ostream& os, size_t buf3Index) {
    buf4[0] = (buf3[0] & 0xFC) >> 2;
    buf4[1] = ((buf3[0] & 0x03) << 4) + ((buf3[1] & 0xF0) >> 4);
    buf4[2] = ((buf3[1] & 0x0F) << 2) + ((buf3[2] & 0xC0) >> 6);
    buf4[3] = buf3[2] & 0x3F;

    for (auto i = 0; i <= buf3Index; i++) {
        os << CHARS[buf4[i]];
    }
}

void Base64::buf4ToBuf3(Buf3& buf3, Buf4& buf4, std::ostream& os, size_t buf4Index) {
    for (auto i = 0; i < buf4Index; i++) {
        buf4[i] = CHAR_INDEXES[buf4[i]];
    }

    buf3[0] = (buf4[0] << 2) + ((buf4[1] & 0x30) >> 4);
    buf3[1] = ((buf4[1] & 0xf) << 4) + ((buf4[2] & 0x3c) >> 2);
    buf3[2] = ((buf4[2] & 0x3) << 6) + buf4[3];

    for (auto i = 0; i < (buf4Index - 1); i++) {
        os << buf3[i];
    }
}
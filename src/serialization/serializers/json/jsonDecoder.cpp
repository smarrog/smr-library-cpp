#include "jsonDecoder.hpp"

#include "serialization/serializationUtils.hpp"
#include "serialization/exceptions/tokenException.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace smr;

Serializable JsonDecoder::decode() {
    skipSpaces(_is);
    switch (_is.peek()) {
        case '[':
            return decodeArray();
        case '{':
            return decodeObject();
        case '"':
            return decodeString();
        case 't':
            checkWord(_is, "true");
            return true;
        case 'f':
            checkWord(_is, "false");
            return false;
        case 'n':
            checkWord(_is, "null");
            return nullptr;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return decodeNumber();
        default:
            throw TokenException(_is.peek());
    }
}

Serializable JsonDecoder::decodeArray() {
    checkToken(_is, '[');
    Array result;
    skipSpaces(_is);
    if (skipToken(_is, ']')) {
        return result;
    }
    while (_is.peek() != -1) {
        result.emplace_back(decode());
        skipSpaces(_is);
        if (skipToken(_is, ',')) {
            skipSpaces(_is);
            continue;
        }
        if (skipToken(_is, ']')) {
            return result;
        }
        throw TokenException(_is.peek());
    }
    throw std::runtime_error("Unexpected EOF");
}

Serializable JsonDecoder::decodeObject() {
    checkToken(_is, '{');
    Object result;
    skipSpaces(_is);
    if (skipToken(_is, '}')) {
        return result;
    }
    while (_is.peek() != -1) {
        std::string key = decodeString().asString();
        skipSpaces(_is);
        checkToken(_is, ':');
        skipSpaces(_is);
        result.emplace(move(key), decode());
        skipSpaces(_is);
        if (skipToken(_is, ',')) {
            skipSpaces(_is);
            continue;
        }
        if (skipToken(_is, '}')) {
            return result;
        }
        throw TokenException(_is.peek());
    }
    throw std::runtime_error("Unexpected EOF");
}

Serializable JsonDecoder::decodeString() {
    checkToken(_is, '"');
    std::ostringstream oss;
    while (_is.peek() != -1) {
        auto next = _is.peek();
        if (next == '"') {
            _is.ignore();
            return oss.str();
        } else if (next == '\\') {
            utf8SymbolToStream(oss, getEscaped(_is));
        } else if (next < ' ' && next != '\t' && next != '\r' && next != '\n') {
            throw TokenException(next);
        } else if (next > 0b1111'1101) {
            throw TokenException(next);
        } else if (next < 0b1000'0000) {
            tokenToStream(oss, _is.get());
        } else {
            utf8SymbolToStream(oss, getUtf8Symbol(_is));
        }
    }
    throw std::runtime_error("Unexpected EOF");
}

Serializable JsonDecoder::decodeNumber() {
    bool isNegative = _is.peek() == '-';
    if (isNegative) {
        _is.ignore();
    }
    auto left = 0ull;
    while (_is.peek() != -1) {
        auto next = _is.peek();
        if (std::isdigit(next)) {
            left = (left << 3) + (left << 1) + next - 48;
            _is.ignore();
        } else if (next == '.' || next == 'e' || next == 'E') {
            std::ostringstream right;
            readFractal(right, _is);
            return std::stod((isNegative ? '-' : '+') + std::string(std::to_string(left) + right.str()));
        } else if (left > INT32_MAX) {
            return isNegative ? -static_cast<double>(left) : static_cast<double>(left);
        } else {
            return isNegative ? -static_cast<int>(left) : static_cast<int>(left);
        }
    }
    throw std::runtime_error("Unexpected EOF");
}
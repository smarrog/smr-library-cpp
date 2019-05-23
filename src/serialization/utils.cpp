#include "utils.hpp"

void smr::tokenToStream(std::ostream& os, int token) {
    os << static_cast<char>(token);
}

void smr::checkToken(std::istream& is, char token, const std::string& location) {
    if (is.peek() == token) {
        is.ignore(1);
    } else {
        throw UnexpectedTokenException(is.peek(), location.empty() ? location : (std::string(" in ") + location + std::string(" while waiting for ") + std::to_string(token)));
    }
}

size_t smr::utf8SymbolToStream(std::ostream& os, uint32_t symbol) {
    if (symbol <= 0b0111'1111) { // 7bit -> 1byte
        tokenToStream(os, symbol);
        return 1;
    }
    else if (symbol <= 0b0111'1111'1111) { // 11bit -> 2byte
        tokenToStream(os, 0b1100'0000 | (0b0001'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
        return 2;
    }
    else if (symbol <= 0b1111'1111'1111'1111) { // 16bit -> 3byte
        tokenToStream(os, 0b1110'0000 | (0b0000'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
        return 3;
    }
    else if (symbol <= 0b0001'1111'1111'1111'1111'1111) { // 21bit -> 4byte
        tokenToStream(os, 0b1111'0000 | (0b0000'0111 & (symbol >> 18)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
        return 4;
    }
    else if (symbol <= 0b0011'1111'1111'1111'1111'1111'1111) { // 26bit -> 5byte
        tokenToStream(os, 0b1111'1000 | (0b0000'0011 & (symbol >> 24)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 18)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
        return 5;
    }
    else if (symbol <= 0b0111'1111'1111'1111'1111'1111'1111'1111) { // 31bit -> 6byte
        tokenToStream(os, 0b1111'1100 | (0b0000'0001 & (symbol >> 30)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 24)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 18)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
        return 6;
    }
    throw std::runtime_error(std::string("Can't pu utf8Symbol ") + std::to_string(symbol) + std::string(" to stream"));
}

uint32_t smr::getUtf8Symbol(std::istream& is) {
    auto next = is.get();
    int bytes;
    uint32_t symbol = 0;
    if ((next & 0b11111100) == 0b11111100) {
        bytes = 6;
        symbol = static_cast<uint8_t>(next) & static_cast<uint8_t>(0b1);
    } else if ((next & 0b11111000) == 0b11111000) {
        bytes = 5;
        symbol = static_cast<uint8_t>(next) & static_cast<uint8_t>(0b11);
    } else if ((next & 0b11110000) == 0b11110000) {
        bytes = 4;
        symbol = static_cast<uint8_t>(next) & static_cast<uint8_t>(0b111);
    } else if ((next & 0b11100000) == 0b11100000) {
        bytes = 3;
        symbol = static_cast<uint8_t>(next) & static_cast<uint8_t>(0b1111);
    } else if ((next & 0b11000000) == 0b11000000) {
        bytes = 2;
        symbol = static_cast<uint8_t>(next) & static_cast<uint8_t>(0b11111);
    } else {
        return next;
    }
    while (--bytes && is.peek() != -1) {
        next = is.get();
        if ((next & 0b11000000) != 0b10000000) {
            throw UnexpectedTokenException(next);
        }
        symbol = (symbol << 6) | (next & 0b0011'1111);
    }
    return symbol;
}

uint32_t smr::getUnicodeSymbol(std::istream& is) {
    uint32_t value = 0;
    for (int i = 0; i < 4; i++) {
        if (is.peek() == -1) {
            throw UnexpectedEofException("unicode");
        }
        auto next = is.get();
        if (next >= '0' && next <= '9') {
            value = (value << 4) | ((next - '0') & 0x0F);
        } else if (next >= 'A' && next <= 'F') {
            value = (value << 4) | ((10 + next - 'A') & 0x0F);
        } else if (next >= 'a' && next <= 'f') {
            value = (value << 4) | ((10 + next - 'a') & 0x0F);
        } else {
            throw UnexpectedTokenException(next);
        }
    }
    return value;
}

uint32_t smr::getEscapedSymbol(std::istream& is) {
    checkToken(is, '\\');
    if (is.peek() == -1) {
	    throw UnexpectedEofException("escaped");
    }
    auto next = is.get();
    switch (next) {
        case '"':
        case '\\':
        case '/':
        case 'b':
        case 'f':
        case 'n':
        case 't':
        case 'r':
            return next;
        case 'u':
            return getUnicodeSymbol(is);
        default:
            throw UnexpectedTokenException(next);
    }
}

uint32_t smr::getEscaped(std::istream& is) {
    uint32_t symbol = getEscapedSymbol(is);
    if (symbol >= 0xD800 && symbol <= 0xDFFF) { // Суррогатная пара UTF16
        if ((symbol & 0b1111'1100'0000'0000) != 0b1101'1000'0000'0000) {
            throw std::runtime_error("Bad escaped symbol: wrong for first byte of utf-16 surrogate pair");
        }
        uint32_t symbol2 = getEscapedSymbol(is);
        if ((symbol2 & 0b1111'1100'0000'0000) != 0b1101'1100'0000'0000) {
            throw std::runtime_error("Bad escaped symbol: wrong for second byte of utf-16 surrogate pair");
        }
        symbol = ((((symbol >> 6) & 0b1111) + 1) << 16) | ((symbol & 0b11'1111) << 10) | (symbol2 & 0b0011'1111'1111);
    }
    return symbol;
}
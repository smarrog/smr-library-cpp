#include "serializationUtils.hpp"

bool smr::skipToken(std::istream& is, char c) {
    if (is.peek() == c) {
        is.ignore(1);
        return true;
    }
    return false;
}

void smr::checkWord(std::istream& is, const std::string& word) {
    for (auto c : word) {
        if (c != static_cast<char>(is.get())) {
            throw std::runtime_error("Failed to read expected word: " + word);
        }
    }
}

void smr::skipSpaces(std::istream& is) {
    while (!is.eof() && std::isspace(is.peek())) {
        is.ignore();
    }
}

void smr::readExponent(std::ostream& os, std::istream& is) {
    auto next = is.get();
    if (next != 'e' && next != 'E') {
        throw TokenException(next);
    }
    tokenToStream(os, next);
    next = is.get();
    if (next != '-' && next != '+') {
        throw TokenException(next);
    }
    tokenToStream(os, next);
    while (std::isdigit(is.peek())) {
        tokenToStream(os, is.get());
    }
}

void smr::readFractal(std::ostream& os, std::istream& is) {
    auto next = is.peek();
    if (next != '.') {
        readExponent(os, is);
        return;
    }
    tokenToStream(os, '.');
    is.ignore();
    while (std::isdigit(is.peek())) {
        tokenToStream(os, is.get());
    }
    if (is.peek() == 'e' || is.peek() == 'E') {
        readExponent(os, is);
    }
}

void smr::utf8SymbolToStream(std::ostream& os, uint32_t symbol) {
    if (symbol <= 0b0111'1111) { // 7bit -> 1byte
        tokenToStream(os, symbol);
    }
    else if (symbol <= 0b0111'1111'1111) { // 11bit -> 2byte
        tokenToStream(os, 0b1100'0000 | (0b0001'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
    }
    else if (symbol <= 0b1111'1111'1111'1111) { // 16bit -> 3byte
        tokenToStream(os, 0b1110'0000 | (0b0000'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
    }
    else if (symbol <= 0b0001'1111'1111'1111'1111'1111) { // 21bit -> 4byte
        tokenToStream(os, 0b1111'0000 | (0b0000'0111 & (symbol >> 18)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
    }
    else if (symbol <= 0b0011'1111'1111'1111'1111'1111'1111) { // 26bit -> 5byte
        tokenToStream(os, 0b1111'1000 | (0b0000'0011 & (symbol >> 24)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 18)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
    }
    else if (symbol <= 0b0111'1111'1111'1111'1111'1111'1111'1111) { // 31bit -> 6byte
        tokenToStream(os, 0b1111'1100 | (0b0000'0001 & (symbol >> 30)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 24)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 18)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 12)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 6)));
        tokenToStream(os, 0b1000'0000 | (0b0011'1111 & (symbol >> 0)));
    }
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
        throw TokenException(next);
    }
    while (--bytes && is.peek() != -1) {
        next = is.get();
        if ((next & 0b11000000) != 0b10000000) {
            throw TokenException(next);
        }
        symbol = (symbol << 6) | (next & 0b0011'1111);
    }
    return symbol;
}

uint32_t smr::getUnicodeSymbol(std::istream& is) {
    uint32_t value = 0;
    for (int i = 0; i < 4; i++) {
        if (is.peek() == -1) {
            throw std::runtime_error("Unexpected eof");
        }
        auto next = is.get();
        if (next >= '0' && next <= '9') {
            value = (value << 4) | ((next - '0') & 0x0F);
        } else if (next >= 'A' && next <= 'F') {
            value = (value << 4) | ((10 + next - 'A') & 0x0F);
        } else if (next >= 'a' && next <= 'f') {
            value = (value << 4) | ((10 + next - 'a') & 0x0F);
        } else {
            throw TokenException(next);
        }
    }
    return value;
}

uint32_t smr::getEscapedSymbol(std::istream& is) {
    checkToken(is, '\\');
    if (is.peek() == -1) {
        throw std::runtime_error("Unexpected eof");
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
            throw TokenException(next);
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
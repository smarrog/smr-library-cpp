#pragma once

#include <iostream>
#include <sstream>
#include <unordered_set>
#include <serialization/exceptions/tokenException.hpp>

namespace smr {
    template <typename T>
    void tokenToStream(std::ostream& os, T token) {
        os << static_cast<char>(token);
    }

    template <typename T>
    void tokenToStream(std::ostream& os, T token, size_t amount) {
        for (auto i = 0; i < amount; i++) {
            os << static_cast<char>(token);
        }
    }

    template <typename T>
    void checkToken(std::istream& is, T token) {
        if (is.peek() == token) {
            is.ignore(1);
        } else {
            throw TokenException(is.peek());
        }
    }

    template <typename T>
    void checkToken(std::istream& is, T token, const std::string& location) {
        try {
            checkToken(is, token);
        } catch (...) {
            throw TokenException(is.peek(), std::string(" in ") + location + std::string(" while waiting for ") + std::to_string(token));
        }
    }

    void skipUntil(std::istream& is, char token);
    void skipUntil(std::istream& is, std::unordered_set<char> tokens);
    std::string readUntil(std::istream& is, char token);
    std::string readUntil(std::istream& is, std::unordered_set<char> tokens);

    void utf8SymbolToStream(std::ostream& os, uint32_t symbol);

    bool skipToken(std::istream& is, char c);
    void checkWord(std::istream& is, const std::string& word);
    void skipSpaces(std::istream& is);

    void readExponent(std::ostream& os, std::istream& is);
    void readFractal(std::ostream& os, std::istream& is);

    uint32_t getUtf8Symbol(std::istream& is);
    uint32_t getUnicodeSymbol(std::istream& is);
    uint32_t getEscapedSymbol(std::istream& is);
    uint32_t getEscaped(std::istream& is);
}
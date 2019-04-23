#pragma once

#include <iostream>

namespace smr {
    template <typename T>
    std::string token(T token) {
        return std::string(">") + std::to_string(token) + std::string("<");
    }

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

    void utf8SymbolToStream(std::ostream& os, uint32_t symbol);

    void checkToken(std::istream& is, char c);
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
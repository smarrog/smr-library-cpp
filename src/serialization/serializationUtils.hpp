#pragma

#include "serialization/exceptions.hpp"

#include <iostream>
#include <sstream>
#include <unordered_set>

namespace smr {
    void tokenToStream(std::ostream& os, int token);
    void checkToken(std::istream& is, char token, const std::string& description = std::string());

    void utf8SymbolToStream(std::ostream& os, uint32_t symbol);

    uint32_t getUtf8Symbol(std::istream& is);
    uint32_t getUnicodeSymbol(std::istream& is);
    uint32_t getEscapedSymbol(std::istream& is);
    uint32_t getEscaped(std::istream& is);
}
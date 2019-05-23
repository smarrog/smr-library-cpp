#pragma

#include "serialization/exceptions.hpp"

#include <iostream>
#include <sstream>
#include <unordered_set>

namespace smr {
    void tokenToStream(std::ostream& os, int token);
    void checkToken(std::istream& is, char token, const std::string& description = std::string());

    size_t utf8SymbolToStream(std::ostream& os, uint32_t symbol);

    uint32_t getUtf8Symbol(std::istream& is);
    uint32_t getUnicodeSymbol(std::istream& is);
    uint32_t getEscapedSymbol(std::istream& is);
    uint32_t getEscaped(std::istream& is);

    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    T getUintFromStream(std::istream& is) {
        T value;
        // TODO BIG ENDIAN or LITTLE ENDIAN ?
        is.read(reinterpret_cast<char*>(&value), sizeof(value));
        return value;
    }
}
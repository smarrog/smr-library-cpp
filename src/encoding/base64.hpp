#pragma once

#include <sstream>
#include <array>

namespace smr {
    class Base64 {
    public:
        static std::string encode(const std::string& s);
        static std::string decode(const std::string& s);

    private:
        using Buf3 = std::array<char, 3>;
        using Buf4 = std::array<char, 4>;

        static void buf3ToBuf4(Buf3& buf3, Buf4& buf4, std::ostream& os, size_t buf3Index);
        static void buf4ToBuf3(Buf3& buf3, Buf4& buf4, std::ostream& os, size_t buf4Index);
    };
}
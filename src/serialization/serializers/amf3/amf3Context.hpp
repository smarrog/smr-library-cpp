#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "amf3Traits.hpp"

namespace smr {
    class Amf3Context {
    public:
        const std::string& getString(size_t index);
        int32_t putString(const std::string& string);
        int32_t getIndex(const std::string& string);

        const Amf3Traits& getTraits(size_t index);
        int32_t putTraits(const Amf3Traits& traits);
        int32_t getIndex(const Amf3Traits& traits);

    private:
        std::vector<std::string> _strings;
        std::unordered_map<std::string, size_t> _stringToIndex;
        std::vector<Amf3Traits> _traits;
    };
}
#include "amf3Context.hpp"

#include <algorithm>

using namespace smr;

const std::string& Amf3Context::getString(size_t index) {
    if (index >= _strings.size()) {
        throw std::runtime_error("No string with index = " + std::to_string(index));
    }
    return _strings.at(index);
}

int32_t Amf3Context::putString(const std::string& string) {
    _strings.emplace_back(string);
    auto index = _strings.size() - 1;
    _stringToIndex[string] = index;
    return index;
}

int32_t Amf3Context::getIndex(const std::string& string) {
    auto it = _stringToIndex.find(string);
    return it != _stringToIndex.end() ? it->second : -1;
}

const Amf3Traits& Amf3Context::getTraits(size_t index) {
    if (index >= _traits.size()) {
        throw std::runtime_error("No traits with index = " + std::to_string(index));
    }
    return _traits.at(index);
}

int32_t Amf3Context::putTraits(const Amf3Traits& traits) {
    _traits.push_back(traits);
    return _traits.size() - 1;
}

int32_t Amf3Context::getIndex(const Amf3Traits& traits) {
    auto it = std::find(_traits.begin(), _traits.end(), traits);
    return it != _traits.end() ? it - _traits.begin() : -1;
}
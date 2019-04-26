#include "iniEncoder.hpp"

using namespace smr;

void IniEncoder::encode(const Serializable& serializable) {
    if (!serializable.isObject()) {
        throw std::runtime_error("Value is not object");
    }
    auto isFirst = true;
    for (auto& [ name, data ] : serializable.asObject()) {
        if (!data.isObject()) {
            throw std::runtime_error("Section data is not object");
        }
        if (!isFirst) {
            _os << '\n';
            if (_config.flags.isFlagSet(Serializer::PRETTY)) {
                _os << '\n';
            }
        }
        isFirst = false;
        encodeSection(name, data.asObject());
    }
}

void IniEncoder::encodeSection(const std::string& name, const Object& data) {
    _os << '[' << name << "]";
    for (auto& [ name, value ] : data) {
        if (!value.isString()) {
            throw std::runtime_error("Multiple values are not supported");
        }
        _os << '\n' << name << '=' << value.asString();
    }
}
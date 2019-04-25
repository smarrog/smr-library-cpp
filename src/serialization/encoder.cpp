#include "encoder.hpp"

using namespace smr;

void Encoder::encode(const Serializable& serializable) {
    if (serializable.isArray()) {
        encode(serializable.asArray());
    } else if (serializable.isObject()) {
        encode(serializable.asObject());
    } else if (serializable.isString()) {
        encode(serializable.asString());
    } else if (serializable.isFloat()) {
        encode(serializable.asFloat());
    } else if (serializable.isInt()) {
        encode(serializable.asInt());
    } else if (serializable.isBool()) {
        encode(serializable.asBool());
    } else if (serializable.isNull()) {
        encode(nullptr);
    } else {
        throw std::runtime_error("Unsupported serializable value");
    }
}
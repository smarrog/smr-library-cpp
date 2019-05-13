#include "amf3Encoder.hpp"
#include "serialization/exceptions.hpp"

using namespace smr;

void Amf3Encoder::encode(const Serializable& serializable) {
    throw NotImplementException("encode");
}

int Amf3Encoder::encode29BitInt(int value) {
    return value;
}
#include "serialization/json/jsonSerializer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace smr;

int main() {
    auto serializer = SerializerFactory::build("json", Serializer::PRETTY | Serializer::ESCAPED_UNICODE);

    std::fstream is("input.txt");
    std::ofstream os("output.txt");
    serializer->encode(os, serializer->decode(is));

    return 0;
}
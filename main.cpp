#include "serialization/serializerFactory.hpp"
#include "compression/compressorFactory.hpp"
#include "commands/command.hpp"
#include "encoding/base64.hpp"
#include "hash/crc32.hpp"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <serialization/serializers/json/jsonSerializer.hpp>
#include <serialization/serializers/dsv/dsvSerializer.hpp>

using namespace smr;

void dsvTest() {
    auto serializer = DsvSerializer();
    serializer.getConfig().separator = ',';
    std::string s = "1,2,3\n4,5,6\n7,8,9";
    std::istringstream iss(s);
    std::ostringstream oss;
    serializer.encode(oss, serializer.decode(iss));
    assert(oss.str() == s);
}

void iniTest() {
    auto serializer = SerializerFactory::build("ini");
    serializer->getConfig().flags.setFlag(serialization::flags::PRETTY);

    std::ifstream is("input.ini");
    std::ofstream os("output.ini");

    serializer->encode(os, serializer->decode(is));
}

void jsonTest() {
    auto serializer = JsonSerializer();
    serializer.getConfig().flags.setFlags(serialization::flags::PRETTY | serialization::flags::ESCAPED_UNICODE);

    std::ifstream is("input.json");
    std::ofstream os("output.json");

    serializer.encode(os, serializer.decode(is));
}

void amf3test() {
    auto jsSerializer = SerializerFactory::build("json");
    jsSerializer->getConfig().flags.setFlags(serialization::flags::PRETTY | serialization::flags::ESCAPED_UNICODE);
    auto amf3Serializer = SerializerFactory::build("amf3");

    std::ifstream is("input.json");
    std::ofstream os("output.json");

    std::fstream fs("tmp.bin");
    std::ifstream libs("libs.bin");

    //jsSerializer->encode(os, jsSerializer->decode(is));

    jsSerializer->encode(os, amf3Serializer->decode(libs));

    //amf3Serializer->encode(fs, jsSerializer->decode(is));
    //jsSerializer->encode(os, amf3Serializer->decode(fs));
}

void flagTest() {
    Flags flags(1 << 1);
    assert(!flags.isFlagSet(1 << 0));
    assert(flags.isFlagSet(1 << 1));
    assert(!flags.isFlagSet(1 << 2));

    flags.setFlag(1 << 1);
    assert(!flags.isFlagSet(1 << 0));
    assert(flags.isFlagSet(1 << 1));
    assert(!flags.isFlagSet(1 << 2));

    flags.unsetFlag(1 << 1);
    assert(!flags.isFlagSet(1 << 0));
    assert(!flags.isFlagSet(1 << 1));
    assert(!flags.isFlagSet(1 << 2));

    flags.unsetFlag(1 << 1);
    assert(!flags.isFlagSet(1 << 0));
    assert(!flags.isFlagSet(1 << 1));
    assert(!flags.isFlagSet(1 << 2));

    flags.setFlag(1 << 1);
    flags.setFlag(1 << 2);
    assert(!flags.isFlagSet(1 << 0));
    assert(flags.isFlagSet(1 << 1));
    assert(flags.isFlagSet(1 << 2));

    flags.reset();
    assert(!flags.isFlagSet(1 << 0));
    assert(!flags.isFlagSet(1 << 1));
    assert(!flags.isFlagSet(1 << 2));
}

void base64Test() {
    assert(Base64::decode(Base64::encode("Hello world")) == "Hello world");
}

void crc32Test() {
    assert(Crc32::hash("01_main") == 3724993606);
    assert(Crc32::hash("02_archive") == 1328046482);
}

void testCommands() {
    CommandErrors errors1;
    CommandErrors errors2;

    auto i = 0;

    errors1.addError("error 1");
    errors1.addError("error 2", [&i](const CommandError error){
        i++;
    });
    errors1.callHandlers();

    errors2.addError("error 3");
    errors2.addErrors(errors1);
    errors2.callHandlers();

    assert(i == 2);
}

void testHuf() {
    auto compressor = CompressorFactory::build("huf");
    std::string s = "ServerGrove";
    std::istringstream iss(s);
    std::stringstream ss;
    std::ostringstream oss;
    compressor->inflate(iss, ss);
    compressor->deflate(ss, oss);
    assert(oss.str() == s);
}

void testTree() {

}

int main() {
    flagTest();
    base64Test();
    crc32Test();
    testCommands();

    //jsonTest();
    //amf3test();
    //dsvTest();
    //iniTest();

    //testHuf();

    testTree();

    return 0;
}
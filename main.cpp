#include "serialization/serializers/json/jsonSerializer.hpp"

#include "commands/commandErrors.hpp"
#include "commands/orCommand.hpp"
#include "commands/andCommand.hpp"

#include "encoding/morton.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace smr;

void testDsv() {
	auto serializer = SerializerFactory::build("dsv");
	std::stringstream ss("1,2,3\n4,5,6\n7,8,9\n\n\n");
	serializer->encode(std::cout, serializer->decode(ss));
}

void testJson() {
	auto serializer = SerializerFactory::build("json");

	std::ifstream is("input.json");
	std::ofstream os("output.json");

	serializer->encode(os, serializer->decode(is));
}

void testXml() {
	auto serializer = SerializerFactory::build("xml");

	std::ifstream is("input.xml");
	std::ofstream os("output.xml");

	serializer->encode(os, serializer->decode(is));
}

void testCommands() {
	CommandErrors errors1;
	CommandErrors errors2;

	errors1.addError("error 1");
	errors1.addError("error 2", [](const CommandError error){
		std::cout << "handle error ";
		std::cout << error.getMessage() << std::endl;
	});
	errors1.callHandlers();

	errors2.addError("error 3");
	errors2.addErrors(errors1);
	errors2.callHandlers();
}

int main() {
	//testJson();
	//testDsv();
	testXml();
	//testCommands();

    return 0;
}
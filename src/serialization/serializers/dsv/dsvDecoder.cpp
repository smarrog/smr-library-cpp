#include "dsvDecoder.hpp"

using namespace smr;

Serializable DsvDecoder::decode() {
	std::string line;
	std::getline(_is, line, '\n');
	Array array{ { decodeLine(line, _config.separator) } };
	_expectedSize = array[0].asArray().size();
	while (_is.peek() != -1) {
		std::getline(_is, line, '\n');
		array.emplace_back(decodeLine(line, _config.separator));
	}
	return array;
}

Array DsvDecoder::decodeLine(std::string_view sv, char separator) {
	Array array;
	int pos = 0;
	while (!sv.empty() && pos != std::string_view::npos) {
		pos = sv.find(separator);
		array.emplace_back(std::string((sv.substr(0, pos))));
		sv.remove_prefix(pos + 1);
	}
	if (_expectedSize != -1 && _expectedSize != array.size()) {
		throw std::runtime_error(std::string("Wrong line size. Expect: ") + std::to_string(_expectedSize) + " got: " + std::to_string(array.size()));
	}
	return array;
}

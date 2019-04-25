#include "dsvEncoder.hpp"

using namespace smr;

void DsvEncoder::encode(std::nullptr_t) {
	throw std::runtime_error("Not implemented");
}

void DsvEncoder::encode(bool value) {
	throw std::runtime_error("Not implemented");
}

void DsvEncoder::encode(int value) {
	throw std::runtime_error("Not implemented");
}

void DsvEncoder::encode(double value) {
	throw std::runtime_error("Not implemented");
}

void DsvEncoder::encode(const std::string& value) {
	throw std::runtime_error("Not implemented");
}

void DsvEncoder::encode(const Array& value) {
	auto isFirstRow = true;
	bool isFirstCol;
	for (auto& rowElem : value) {
		if (!isFirstRow) {
			_os << '\n';
		}
		isFirstRow = false;
		isFirstCol = true;
		for (auto& elem : rowElem.asArray()) {
			if (!isFirstCol) {
				_os << ',';
			}
			isFirstCol = false;
			_os << elem.asString();
		}
	}
}

void DsvEncoder::encode(const Object& value) {
	throw std::runtime_error("Not implemented");
}
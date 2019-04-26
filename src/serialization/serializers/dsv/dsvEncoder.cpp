#include "dsvEncoder.hpp"

using namespace smr;

void DsvEncoder::encode(const Serializable& serializable) {
	if (!serializable.isArray()) {
		throw std::runtime_error("Value is not array");
	}
	auto isFirstRow = true;
	bool isFirstCol;
	for (auto& rowElem : serializable.asArray()) {
		if (!isFirstRow) {
			_os << '\n';
		}
		isFirstRow = false;
		isFirstCol = true;
		for (auto& elem : rowElem.asArray()) {
			if (!isFirstCol) {
				_os << _config.separator;
			}
			isFirstCol = false;
			_os << elem.asString();
		}
	}
}
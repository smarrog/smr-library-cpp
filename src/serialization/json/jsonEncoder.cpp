#include "jsonEncoder.hpp"

#include "serialization/serializationUtils.hpp"
#include "../exceptions/tokenException.hpp"

using namespace smr;

void JsonEncoder::encode(const Serializable& serializable) {
	if (serializable.isNull()) {
		return encode(serializable.asNull());
	} else if (serializable.isBool()) {
		return encode(serializable.asBool());
	} else if (serializable.isInt()) {
		return encode(serializable.isInt());
	} else if (serializable.isFloat()) {
		return encode(serializable.asFloat());
	} else if (serializable.isString()) {
		return encode(serializable.asString());
	} else if (serializable.isArray()) {
		return encode(serializable.asArray());
	} else if (serializable.isObject()) {
		return encode(serializable.asObject());
	}
}

void JsonEncoder::encode(std::nullptr_t) {
    _os << "null";
}

void JsonEncoder::encode(bool value) {
    _os << (value ? "true" : "false");
}

void JsonEncoder::encode(int value) {
    _os << std::dec;
    _os << value;
}

void JsonEncoder::encode(double value) {
    _os << std::setprecision(std::numeric_limits<double>::digits10 + 1) << std::dec;
    _os << value;
}

void JsonEncoder::encode(const std::string& value) {
    _os << '"';
    std::istringstream iss(value);
    while (iss.peek() != -1) {
        auto next = iss.get();
        switch (next) {
            case '"':
                _os << "\\\"";
                break;
            case '\\':
                _os << "\\\\";
                break;
            case '/':
                _os << "\\/";
                break;
            case '\b':
                _os << "\\b";
                break;
            case '\f':
                _os << "\\f";
                break;
            case '\n':
                _os << "\\n";
                break;
            case '\t':
                _os << "\\t";
                break;
            case '\r':
                _os << "\\r";
                break;
            default:
                if (!_config.flags.isFlagSet(Serializer::ESCAPED_UNICODE)) {
                    tokenToStream(_os, next);
                    break;
                }
                if (next > 0b1111'1101) {
                    throw TokenException(next);
                } else if (next < 0b1000'0000) {
                    tokenToStream(_os, next);
                } else {
                    iss.putback(next);
                    _os << "\\u" << std::setw(4) << std::setfill('0') << std::hex << getUtf8Symbol(iss);
                }
        }
    }
    _os << '"';
}

template <typename Container, typename ElemEncoder>
void JsonEncoder::encodeContainer(const Container& container, char open, char close, ElemEncoder elemEncoder) {
    if (container.empty()) {
        _os << open << close;
        return;
    }
    _os << open;
    _offset++;
    nextLine();
    auto isFirst = true;
    for (const auto& elem : container) {
        if (!isFirst) {
            _os << ',';
            nextLine();
        }
        isFirst = false;
        elemEncoder(elem);
    }
    _offset--;
    nextLine();
    _os << close;
}

void JsonEncoder::encode(const Array& value) {
    encodeContainer(value, '[', ']', [this](const auto& elem) {
        encode(elem);
    });
}

void JsonEncoder::encode(const Object& value) {
    encodeContainer(value, '{', '}', [this](const auto& elem) {
        _os << '"' << elem.first << "\":";
        addSpacing();
        encode(elem.second);
    });
}

void JsonEncoder::nextLine() {
	if (_config.flags.isFlagSet(Serializer::PRETTY)) {
		_os << '\n';
		tokenToStream(_os, '\t', _offset);
	}
}

void JsonEncoder::addSpacing() {
	if (_config.flags.isFlagSet(Serializer::PRETTY)) {
		tokenToStream(_os, ' ');
	}
}
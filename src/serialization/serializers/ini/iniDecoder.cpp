#include "iniDecoder.hpp"

#include "serialization/serializationUtils.hpp"
#include "serialization/exceptions.hpp"

using namespace smr;

Serializable IniDecoder::decode() {
    _sections = {};
    std::string line;
    while (_is.peek() != -1) {
        std::getline(_is, line);
        decodeLine(line);
    }
    return std::move(_sections);
}

void IniDecoder::decodeLine(std::string_view line) {
    while (line[0] == ' ') {
        line.remove_prefix(1);
    }
    if (line.empty()) {
        _sectionPtr = nullptr;
        return;
    }
    if (line[0] == ';' || line[0] == '#') {
        return;
    }
    if (!_sectionPtr || line[0] == '[') {
        if (line[0] != '[') {
            throw UnexpectedTokenException('[', " on parsing header");
        }
        line.remove_prefix(1);
        auto pos = line.find(']');
        if (pos == std::string_view::npos) {
            throw std::runtime_error("No closing of header");
        }
        auto sectionName = std::string(line.substr(0, pos));
        if (_sections.find(sectionName) == _sections.end()) {
            _sections[sectionName] = Object{};
        }
        _sectionPtr = &(_sections[sectionName].asObject());
    } else {
        auto pos = line.find('=');
        if (pos == std::string_view::npos) {
            throw std::runtime_error("No '=' token in value line");
        }
        std::string_view varName = line.substr(0, pos);
        line.remove_prefix(pos + 1);
        std::string_view varValue = line.substr(0, std::min(line.find(';'), line.find('#')));
        _sectionPtr->insert({std::string(varName), std::string(varValue)});
    }
}
#include "string.hpp"

#include <cctype>

using namespace smr;

std::pair<std::string_view, std::string_view> String::split(std::string_view str, char by) {
	auto pos = str.find(by);
	return { str.substr(0, pos), (pos < str.size() && pos + 1 < str.size()) ? str.substr(pos + 1) : std::string_view() };
}

std::string_view String::strip(std::string_view str) {
	return lStrip(rStrip(str));
}

std::string_view String::lStrip(std::string_view str) {
	while (!str.empty() && std::isspace(str[0])) {
		str.remove_prefix(1);
	}
	return str;
}

std::string_view String::rStrip(std::string_view str) {
	while (!str.empty() && std::isspace(str[str.size() - 1])) {
		str.remove_suffix(1);
	}
	return str;
}

std::string_view String::unquote(std::string_view str) {
	if (!str.empty() && str.front() == '"') {
		str.remove_prefix(1);
	}
	if (!str.empty() && str.back() == '"') {
		str.remove_suffix(1);
	}
	return str;
}
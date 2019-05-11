#pragma once

#include <string_view>

namespace smr {
	class String {
	public:
		String() = delete;

		static std::pair<std::string_view, std::string_view> split(std::string_view line, char by);
		static std::string_view strip(std::string_view line);
		static std::string_view lStrip(std::string_view line);
		static std::string_view rStrip(std::string_view line);
		static std::string_view unquote(std::string_view value);
	};
}

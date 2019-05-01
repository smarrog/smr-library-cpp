#pragma once

#include <string>

namespace smr {
	class CommandError {
	public:
		CommandError(std::string message = "")
			: _message(std::move(message)) {}

		const std::string& getMessage() const {
			return _message;
		}

	private:
		std::string _message;
	};
}

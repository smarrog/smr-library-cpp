#pragma once

#include "commandError.hpp"

#include <vector>
#include <functional>

namespace smr {
	class CommandErrors {
	public:
		using Handler = std::function<void(const CommandError&)>;

		void addError(std::string message);
		void addError(std::string message, Handler handler);
		void addErrors(const CommandErrors& errors);
		void callHandlers() const;
		void reset();

		bool hasUnhandledError() const {
			return !_unhandledErrors.empty();
		}

		bool hasError() const {
			return !_unhandledErrors.empty() || !_handledErrors.empty();
		}

		std::string getMessage() const;

	private:
		std::vector<CommandError> _unhandledErrors;
		std::vector<std::pair<CommandError, Handler>> _handledErrors;
	};
}

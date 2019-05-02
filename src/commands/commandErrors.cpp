#include "commandErrors.hpp"

using namespace smr;

void CommandErrors::addError(std::string message) {
	_unhandledErrors.emplace_back(std::move(message));
}

void CommandErrors::addError(std::string message, Handler handler) {
	_handledErrors.emplace_back(std::move(message), handler);
}

void CommandErrors::addErrors(const CommandErrors& errors) {
	for (auto& error : errors._unhandledErrors) {
		addError(error.getMessage());
	}
	for (auto& [ error, handler ] : errors._handledErrors) {
		addError(error.getMessage(), handler);
	}
}

void CommandErrors::callHandlers() const {
	for (auto& [ error, handler ] : _handledErrors) {
		handler(error);
	}
}

void CommandErrors::reset() {
	_unhandledErrors.clear();
	_handledErrors.clear();
}

std::string CommandErrors::getMessage() const {
	if (!hasError()) {
		return "no errors";
	}
	return std::string("Unhandled errors: ") + std::to_string(_unhandledErrors.size());
}
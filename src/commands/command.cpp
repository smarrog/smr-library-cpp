#include "command.hpp"

#include <stdexcept>

using namespace smr;

bool Command::execute() {
	auto errors = createErrors();
	onBeforeExecute();
	if (canExecuteWith(errors)) {
		executeInternal();
		onAfterExecute();
		return true;
	}
	if (canHandleWith(errors)) {
		errors.callHandlers();
	}
	onAfterExecute();
	return false;
}

CommandErrors Command::createErrors() {
	CommandErrors errors;
	try {
		generateErrors(errors);
	} catch (const std::exception& exception) {
		throw std::runtime_error(std::string("Can't generate errors -> ") + exception.what());
	}
	return errors;
}
#include "orCommand.hpp"

using namespace smr;

void OrCommand::generateErrors(CommandErrors& errors) {
	bool hasCommandToExecute = _commands.empty();
	for (auto& command : _commands) {
		if (command->canExecute()) {
			hasCommandToExecute = true;
			break;
		}
	}
	if (!hasCommandToExecute) {
		errors.addError("execute block");
	}
}

void OrCommand::executeInternal() {
	for (auto& command : _commands) {
		if (command->canExecute()) {
			command->execute();
			break;
		}
	}
}
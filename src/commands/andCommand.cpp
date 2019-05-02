#include "andCommand.hpp"

using namespace smr;

void AndCommand::generateErrors(CommandErrors& errors) {
	for (auto& command : _commands) {
		if (!command->canExecute()) {
			errors.addError("execute block");
			break;
		}
	}
}

void AndCommand::executeInternal() {
	for (auto& command : _commands) {
		command->execute();
	}
}
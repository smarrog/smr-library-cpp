#pragma once

#include "commandErrors.hpp"

#include <stdexcept>

namespace smr {
	class Command {
	public:
		bool static canExecuteWith(const CommandErrors& errors) { return !errors.hasError(); }
		bool static canHandleWith(const CommandErrors& errors) { return !errors.hasUnhandledError(); }

		virtual ~Command() = default;

		bool canExecute() { return canExecuteWith(createErrors()); }
		bool canHandle() { return canHandleWith(createErrors()); }

		CommandErrors createErrors();
		bool execute();

	protected:
		virtual void generateErrors(CommandErrors&) = 0;
		virtual void executeInternal() = 0;
		virtual void onBeforeExecute() {};
		virtual void onAfterExecute() {};
	};
}

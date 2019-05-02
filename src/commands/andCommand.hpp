#pragma once

#include "command.hpp"

#include <vector>
#include <memory>

namespace smr {
	class AndCommand : public Command {
	public:
		explicit AndCommand(std::vector<std::unique_ptr<Command>> commands)
			: _commands(std::move(commands)) {}

	protected:
		void generateErrors(CommandErrors& errors) override;
		void executeInternal() override;

	private:
		std::vector<std::unique_ptr<Command>> _commands;
	};
}
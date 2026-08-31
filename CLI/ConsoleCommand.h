#pragma once

#include <string>
#include <optional>


namespace ConsoleCommand {
	struct CommandResult {
		std::optional<int> port;
		std::optional<std::string> origin;
		bool isClearCache = false;

		operator bool() const { return port || origin || isClearCache; }
	};

	CommandResult Parse(int argc, char* const argv[]);
};
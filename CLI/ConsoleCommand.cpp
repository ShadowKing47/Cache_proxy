#include "ConsoleCommand.h"
#include "CommandLineOptions.h"


#include <iostream>
#include <stdexcept>
#include <boost/program_options.hpp>



namespace ConsoleCommand {
	CommandResult Parse(int argc, char* const argv[]) {
		namespace po = boost::program_options;
		po::options_description desc{ CommandLineOptions::CreateOptionsDescription() };

		try {
			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			if (vm.count("help")) {
				std::cout << desc << "\n";
				return { };
			}

			CommandResult res{ };
			if (vm.count("port")) { res.port = vm["port"].as<int>(); }
			if (vm.count("origin")) { res.origin = vm["origin"].as<std::string>(); }
			if (vm.count("clear-cache")) { res.isClearCache = true; }

			return res;
		}
		catch (const po::error& e) {
			std::cerr << "Error parsing command line: " << e.what() << "\n";
			return { };
		}
		catch (const std::exception& e) {
			std::cerr << "General error: " << e.what() << "\n";
			return { };
		}
	}
}
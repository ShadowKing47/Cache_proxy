#include <iostream>
#include <string>
#include <format>



#include "Listener.h"
#include "ConsoleCommand.h"
#include "CommandLineOptions.h"




int main(int argc, char* const argv[]) {
	auto res{ ConsoleCommand::Parse(argc, argv) };

	if (!res) {
		std::cout << "Enter \"--help\".\n";
		return EXIT_FAILURE;
	}

	
	std::shared_ptr<Cache> cache{ std::make_shared<Cache>() };
	if (res.port && res.origin) {
		// main logic proxy server
		auto port{ static_cast<unsigned short>(res.port.value()) };
		auto const address{ net::ip::make_address("127.0.0.1") };
		auto origin{ res.origin.value() };
			
		std::cout << "[SERVER] --port " << port << " --origin " << origin << "\n";

		net::io_context ioc{ };
		std::make_shared<Listener>(
			ioc,
			tcp::endpoint{ address, port }, 
			origin,
			cache) -> Run();

		ioc.run();
	}
	else if (res.isClearCache) {
		// clear cache get and head request
		std::cout << "Cache cleared\n";
		
	}

	return EXIT_SUCCESS;
}

//caching-proxy --port <number> --origin <url>

//--memory-cache: Should use a Memory Cache that will be cleaned when the process finishes?
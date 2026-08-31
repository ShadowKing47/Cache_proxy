#include "CommandLineOptions.h"

namespace CommandLineOptions {
	po::options_description CreateOptionsDescription() {
		po::options_description desc{ "Allowed options" };
		desc.add_options()
			("port", po::value<int>(), "port on which the caching proxy server will operate.")
			("origin", po::value<std::string>(), "the URL of the server to which requests will be redirected.")
			("clear-cache", "is clear cache.")
			("help", "--port <number> --origin <url>");

		return desc;
	}
}
#pragma once

#include <boost/program_options.hpp>

namespace CommandLineOptions {
	namespace po = boost::program_options;
	po::options_description CreateOptionsDescription();
}
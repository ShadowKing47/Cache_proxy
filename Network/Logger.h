#pragma once

#include <string_view>

#include <boost/beast.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>

namespace Logger {
	// Report a failure
	void Fail(beast::error_code ec, std::string_view what);
}
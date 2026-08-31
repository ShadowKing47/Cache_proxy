#include "Logger.h"

#include <iostream>

void Logger::Fail(beast::error_code ec, std::string_view what) {
	std::cerr << what << ": " << ec.message() << "\n";
}
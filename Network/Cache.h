#pragma once

#include <string>
#include <shared_mutex>
#include <unordered_map>
#include <optional>


#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>

#include <boost/asio/strand.hpp>

#include <boost/config.hpp>




namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class Cache {
public:
	Cache() = default;

	void Set(const std::string& key, const http::response<http::string_body>& value);
	std::optional<http::response<http::string_body>> TryGet(const std::string& key) const;
	bool Contains(const std::string& key) const;

private:

	mutable std::shared_mutex m_mutex;
	std::unordered_map<std::string, http::response<http::string_body>> m_data;
};
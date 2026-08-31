#pragma once

#include <iostream>

#include "Logger.h"
#include "Cache.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>

#include <boost/asio/strand.hpp>

#include <boost/config.hpp>

#include <memory>



namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket&& sock, std::shared_ptr<Cache> cache, std::string origin) noexcept;

	void Run();
	
	void DoRead();

	void OnRead(beast::error_code ec, std::size_t bytes_transferred);

	void SendResponse(http::message_generator&& msg);

	void OnWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred);

	void DoClose();

	template <class Body, class Allocator>
	static inline http::message_generator HandleRequest(
		http::request<Body, http::basic_fields<Allocator>>&& req,
		std::shared_ptr<Cache> cache,
		std::string origin);

private:

	beast::tcp_stream m_stream;
	beast::flat_buffer m_buffer;
	http::request<http::string_body> m_req;
	std::shared_ptr<Cache> m_cache;
	std::string m_host; //uri the server from which we cache responses
};

template <class Body, class Allocator>
static inline http::message_generator Session::HandleRequest(
	http::request<Body, http::basic_fields<Allocator>>&& req,
	std::shared_ptr<Cache> cache, 
	std::string host) {

	// 1 Check in the cache, if there is, then give it immediately
	std::string key{ std::string{ req.method_string() } + std::string{ req.target() } };

	auto item{ cache -> TryGet(key) };
	if (item.has_value()) {
		item.value().set("X-Cache:", "HIT");
		return std::move(item.value());
	}

	// 2 If it is not in the cache, then we establish a connection and send a request.
	// Declare a container to hold the response
	http::response<http::string_body> res{ };
	try {
		// The io_context is required for all I/O
		net::io_context ioc;

		beast::tcp_stream stream(ioc);


		// DNS lookup
		tcp::resolver resolver{ ioc };
		tcp::resolver::query query(host, "80");
		tcp::resolver::iterator iter = resolver.resolve(query);

		tcp::endpoint endpoint = iter -> endpoint();

		// Make the connection on the IP address we get from a lookup
		stream.connect(endpoint);

		
		req.set(http::field::host, host);
		
		// Send the HTTP request to the remote host
		http::write(stream, std::move(req));

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer{ };

		// Receive the HTTP response
		http::read(stream, buffer, res);

		res.set("X-Cache:", "MISS");

		res.prepare_payload();



		// 3 get è head we cache the methods
		if (req.method() == beast::http::verb::get || req.method() == beast::http::verb::head) {
			cache -> Set(key, res);
		}

		// Gracefully close the socket
		beast::error_code ec{ };
		stream.socket().shutdown(tcp::socket::shutdown_both, ec);
		// not_connected happens sometimes so don't bother reporting it.
		if (ec && ec != beast::errc::not_connected) { throw beast::system_error{ ec }; }
		// If we get here then the connection is closed gracefully
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		res.version(11);
		res.result(http::status::internal_server_error);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Internal Server Error";
		res.prepare_payload();
	}

	return std::move(res);
}

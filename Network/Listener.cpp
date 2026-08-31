#include "Listener.h"

#include "Logger.h"
#include "Session.h"



Listener::Listener(net::io_context& ioc,
	tcp::endpoint endpoint,
	std::string origin,
	std::shared_ptr<Cache> cache)
	: m_ioc{ ioc }
	, m_acceptor{ ioc, endpoint }
	, m_host{ origin }
	, m_cache{ cache }
{
	beast::error_code ec;

	// Open the acceptor
	m_acceptor.open(endpoint.protocol(), ec);
	if (ec) {
		Logger::Fail(ec, "open");
		return;
	}

	// Allow address reuse
	m_acceptor.set_option(net::socket_base::reuse_address(true), ec);
	if (ec) {
		Logger::Fail(ec, "set_option");
		return;
	}

	// Bind to the server address
	m_acceptor.bind(endpoint, ec);
	if (ec) {
		Logger::Fail(ec, "bind");
		return;
	}

	// Start listening for connections
	m_acceptor.listen(net::socket_base::max_listen_connections, ec);
	if (ec) {
		Logger::Fail(ec, "listen");
		return;
	}
}

// Start accepting incoming connections
void Listener::Run() {
	DoAccept();
}


void Listener::DoAccept() {
	// The new connection gets its own strand
	m_acceptor.async_accept(
		net::make_strand(m_ioc),
		beast::bind_front_handler(
			&Listener::OnAccept,
			shared_from_this()));
}

void Listener::OnAccept(beast::error_code ec, tcp::socket&& socket) {
	if (ec) {
		return Logger::Fail(ec, "accept"); // To avoid infinite loop
	}
	else
	{
		// Create the session and run it
		std::make_shared<Session>(
			std::move(socket),
			m_cache,
			m_host) -> Run();
	}

	// Accept another connection
	DoAccept();
}
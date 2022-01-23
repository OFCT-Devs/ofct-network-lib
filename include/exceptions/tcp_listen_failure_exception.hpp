#ifndef SERVER_TCP_LISTEN_FAILURE_EXCEPTION_HPP
#define SERVER_TCP_LISTEN_FAILURE_EXCEPTION_HPP

#include <netinet/in.h>

#include <exception>
#include <string>

namespace OFCT::networking {
	class tcp_listen_failure_exception : public std::exception {
	public:
		tcp_listen_failure_exception(in_port_t port, in_addr_t ip)
		  : message("Failed to listen; port = " + std::to_string(port) + ", ip = " + std::to_string(ip)) {}
		[[nodiscard]] char const *what() const noexcept final { return message.c_str(); }
	private:
		std::string message;
	};
}

#endif

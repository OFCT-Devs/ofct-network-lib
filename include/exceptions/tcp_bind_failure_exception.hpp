#ifndef SERVER_TCP_BIND_FAILURE_EXCEPTION_HPP
#define SERVER_TCP_BIND_FAILURE_EXCEPTION_HPP

#include <netinet/in.h>

#include <exception>
#include <string>

namespace OFCT::networking {
	class tcp_bind_failure_exception : public std::exception {
	public:
		tcp_bind_failure_exception(in_port_t port, in_addr_t addr)
		  : message("Failed to bind; port = " + std::to_string(port) + ", ip = " + std::to_string(addr)) {}
		[[nodiscard]] char const *what() const noexcept final { return message.c_str(); }
	private:
		std::string message;
	};
}

#endif

#ifndef SERVER_TCP_INAPPROPRIATE_IP_EXCEPTION_HPP
#define SERVER_TCP_INAPPROPRIATE_IP_EXCEPTION_HPP

#include <netinet/in.h>

#include <exception>
#include <string>

namespace OFCT::networking {
	class tcp_inappropriate_ip_exception : public std::exception {
	public:
		explicit tcp_inappropriate_ip_exception(in_addr_t ip = INADDR_NONE)
		  : message("Got inappropriate IP address " + std::to_string(ip) + ".") {}
		[[nodiscard]] char const *what() const noexcept final { return message.c_str(); }

	private:
		std::string message;
	};
}

#endif

#ifndef OFCT_NETWORK_socket_tcp_socket_hpp
#define OFCT_NETWORK_socket_tcp_socket_hpp

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string_view>

#include "socket_base.hpp"

#include "../exceptions/tcp_exceptions.hpp"

namespace OFCT::networking {

	enum class sockaddr_type {
		NONE,
		IN,
		IN6,
		UN,
	};

	constexpr auto sockaddr_type_none = sockaddr_type::NONE;
	constexpr auto sockaddr_type_in = sockaddr_type::IN;
	constexpr auto sockaddr_type_in6 = sockaddr_type::IN6;
	constexpr auto sockaddr_type_un = sockaddr_type::UN;

	template<sockaddr_type type>
	struct socktype_selector;

	template<>
	struct socktype_selector<sockaddr_type_in> { using type = sockaddr_in; };
	template<>
	struct socktype_selector<sockaddr_type_in6> { using type = sockaddr_in6; };
	template<>
	struct socktype_selector<sockaddr_type_un> { using type = sockaddr_un; };


    template<sockaddr_type type, bool nonblocking>
    class tcp_socket;

	// IPv4
    template<bool nonblocking>
    class tcp_socket<sockaddr_type_in, nonblocking> : public socket_base<domain_inet, type_stream, protocol_default, false> {
	protected:
		using socktype = socktype_selector<sockaddr_type_in>::type;
		static constexpr auto family = sock_domain_to_AF(domain_inet);

	public:
	    explicit tcp_socket()
		  : socket_base<domain_inet, type_stream, protocol_default, false>() {
		    ::memset(&addr, 0, sizeof(addr));
		}

		explicit tcp_socket(in_port_t port, in_addr_t ip)
		  : socket_base<domain_inet, type_stream, protocol_default, false>() {
			addr.sin_family = family;
			addr.sin_port = ::htons(port);
			addr.sin_addr.s_addr = ::htonl(ip);
			::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		}

		explicit tcp_socket(int sockfd, in_port_t port, in_addr_t ip)
		  : socket_base<domain_inet, type_stream, protocol_default, false>(sockfd) {
			addr.sin_family = family;
			addr.sin_port = ::htons(port);
			addr.sin_addr.s_addr = ::htonl(ip);
			::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		}

		explicit tcp_socket(in_port_t port, std::string_view ip_str)
		  : socket_base<domain_inet, type_stream, protocol_default, false>() {
			in_addr_t const ip = inet_addr(ip_str.data());
			if(ip == INADDR_NONE) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to parse IP address: %s\n", ip_str.data());
				}
				throw tcp_inappropriate_ip_exception(ip);
			}
			addr.sin_family = family;
			addr.sin_port = ::htons(port);
			addr.sin_addr.s_addr = ip;
			::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		}

	    explicit tcp_socket(int sockfd, in_port_t port, std::string_view ip_str)
		  : socket_base<domain_inet, type_stream, protocol_default, false>(sockfd) {
		    in_addr_t const ip = inet_addr(ip_str.data());
		    if(ip == INADDR_NONE) {
			    if constexpr(debug_mode) {
				    ::fprintf(stderr, "Failed to parse IP address: %s\n", ip_str.data());
			    }
			    throw tcp_inappropriate_ip_exception(ip);
		    }
		    addr.sin_family = family;
		    addr.sin_port = ::htons(port);
		    addr.sin_addr.s_addr = ip;
		    ::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		}

	protected:
		socktype addr;
	};
}

#endif
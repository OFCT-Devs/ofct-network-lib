#ifndef SERVER_TCP_CLIENT_HPP
#define SERVER_TCP_CLIENT_HPP

#include "tcp_transceiver.hpp"

namespace OFCT::networking {

	template<sockaddr_type type, bool nonblocking>
	class tcp_client;

	// IPv4
	template<bool nonblocking>
	class tcp_client<sockaddr_type_in, nonblocking> : public tcp_transceiver<sockaddr_type_in, false> {
		static constexpr size_t BUFFER_SIZE = tcp_transceiver<sockaddr_type_in, false>::BUFFER_SIZE;

	public:
		explicit tcp_client() : tcp_transceiver<sockaddr_type_in, false>() {}

		explicit tcp_client(in_port_t port, in_addr_t ip)
		  : tcp_transceiver<sockaddr_type_in, false>(port, ip) {}

		explicit tcp_client(int sockfd, in_port_t port, in_addr_t ip)
		  : tcp_transceiver<sockaddr_type_in, false>(sockfd, port, ip) {}

		explicit tcp_client(in_port_t port, std::string_view ip_str)
		  : tcp_transceiver<sockaddr_type_in, false>(port, ip_str) {}

		explicit tcp_client(int sockfd, in_port_t port, std::string_view ip_str)
		  : tcp_transceiver<sockaddr_type_in, false>(sockfd, port, ip_str) {}

		[[nodiscard]] bool connect() const {
			return !::connect(this->sockfd, reinterpret_cast<sockaddr const*>(&this->addr), sizeof(this->addr));
		}
	};
}

#endif

#ifndef SERVER_TCP_TRANSCEIVER_HPP
#define SERVER_TCP_TRANSCEIVER_HPP

#include "tcp_socket.hpp"

#include <vector>

namespace OFCT::networking {

	template<sockaddr_type type, bool nonblocking>
	class tcp_transceiver;

	// blocking
	template<sockaddr_type type>
	class tcp_transceiver<type, false> : public tcp_socket<sockaddr_type_in, false> {
	protected:
		static constexpr size_t BUFFER_SIZE = 1024;

	public:
		explicit tcp_transceiver() : tcp_socket() {}

		explicit tcp_transceiver(in_port_t port, in_addr_t ip)
		  : tcp_socket<sockaddr_type_in, false>(port, ip) {}

		explicit tcp_transceiver(int sockfd, in_port_t port, in_addr_t ip)
		  : tcp_socket<sockaddr_type_in, false>(sockfd, port, ip) {}

		explicit tcp_transceiver(in_port_t port, std::string_view ip_str)
		  : tcp_socket<sockaddr_type_in, false>(port, ip_str) {}

		explicit tcp_transceiver(int sockfd, in_port_t port, std::string_view ip_str)
		  : tcp_socket<sockaddr_type_in, false>(sockfd, port, ip_str) {}

		[[nodiscard]] ssize_t send_raw(void const *buf, size_t len, int flags = 0) const {
			return ::send(this->sockfd, buf, len, flags);
		}

		[[nodiscard]] ssize_t recv_raw(void *buf, size_t len, int flags = 0) const {
			return ::recv(this->sockfd, buf, len, flags);
		}

		[[nodiscard]] bool send(void const *buf, size_t len) const {
			size_t offset = 0;
			while(offset < len) {
				size_t const size = ((len - offset) > BUFFER_SIZE ? BUFFER_SIZE : (len - offset));
				ssize_t const sent = send_raw(reinterpret_cast<uint8_t const*>(buf) + offset, size);
				if(sent == -1) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to send.\n");
					}
					return false;
				}
				offset += sent;
			}
			return true;
		}

		[[nodiscard]] bool send(std::string const &buf) const {
			return send(buf.c_str(), buf.size());
		}

		[[nodiscard]] bool send(std::vector<uint8_t> const &buf) const {
			return send(buf.data(), buf.size());
		}

		[[nodiscard]] bool recv(void *buf, size_t len) const {
			size_t offset = 0;
			while(offset < len) {
				size_t const size = ((len - offset) > BUFFER_SIZE ? BUFFER_SIZE : (len - offset));
				ssize_t const received = recv_raw(reinterpret_cast<uint8_t*>(buf) + offset, size);
				if(received == -1) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to receive.\n");
					}
					return false;
				}
				offset += received;
			}
			return true;
		}

		[[nodiscard]] bool recv(std::string &buf, size_t len) const {
			buf.resize(len);
			return recv(buf.data(), len);
		}

		[[nodiscard]] bool recv(std::vector<uint8_t> &buf, size_t len) const {
			buf.resize(len);
			return recv(buf.data(), len);
		}
	};

	// nonblocking
	template<sockaddr_type type>
	class tcp_transceiver<type, true> : public tcp_socket<sockaddr_type_in, true> {
	protected:
		static constexpr size_t BUFFER_SIZE = 1024;

	public:
		explicit tcp_transceiver() : tcp_socket() {}

		explicit tcp_transceiver(in_port_t port, in_addr_t ip)
				: tcp_socket<sockaddr_type_in, true>(port, ip) {}

		explicit tcp_transceiver(int sockfd, in_port_t port, in_addr_t ip)
				: tcp_socket<sockaddr_type_in, true>(sockfd, port, ip) {}

		explicit tcp_transceiver(in_port_t port, std::string_view ip_str)
				: tcp_socket<sockaddr_type_in, true>(port, ip_str) {}

		explicit tcp_transceiver(int sockfd, in_port_t port, std::string_view ip_str)
				: tcp_socket<sockaddr_type_in, true>(sockfd, port, ip_str) {}

		[[nodiscard]] ssize_t send_raw(void const *buf, size_t len, int flags = 0) const {
			return ::send(this->sockfd, buf, len, flags);
		}

		[[nodiscard]] ssize_t recv_raw(void *buf, size_t len, int flags = 0) const {
			return ::recv(this->sockfd, buf, len, flags);
		}

		[[nodiscard]] bool send(void const *buf, size_t len) const {
			size_t offset = 0;
			while(offset < len) {
				size_t const size = ((len - offset > BUFFER_SIZE ? BUFFER_SIZE : (len - offset)));
				ssize_t const sent = send_raw(reinterpret_cast<uint8_t const*>(buf) + offset, size);
				if(sent == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to send.\n");
					}
					return false;
				}
				offset += sent;
			}
			return true;
		}

		[[nodiscard]] bool send(std::string const &buf) const {
			return send(buf.c_str(), buf.size());
		}

		[[nodiscard]] bool send(std::vector<uint8_t> const &buf) const {
			return send(buf.data(), buf.size());
		}

		[[nodiscard]] bool recv(void *buf, size_t len) const {
			size_t offset = 0;
			while(offset < len) {
				size_t const size = ((len - offset) > BUFFER_SIZE ? BUFFER_SIZE : (len - offset));
				ssize_t const received = recv_raw(reinterpret_cast<uint8_t*>(buf) + offset, size);
				if(received == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to receive.\n");
					}
					return false;
				}
				offset += received;
			}
			return true;
		}

		[[nodiscard]] bool recv(std::string &buf, size_t len) const {
			buf.resize(len);
			return recv(buf.data(), len);
		}

		[[nodiscard]] bool recv(std::vector<uint8_t> &buf, size_t len) const {
			buf.resize(len);
			return recv(buf.data(), len);
		}
	};

}

#endif

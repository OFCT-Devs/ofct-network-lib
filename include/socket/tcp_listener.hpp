#ifndef SERVER_TCP_LISTENER_HPP
#define SERVER_TCP_LISTENER_HPP

#include <atomic>
#include <limits>
#include <thread>

#include "tcp_socket.hpp"
#include "tcp_transceiver.hpp"

namespace OFCT::networking {

	// !!VIRTUAL FUNCTION!!
	template<sockaddr_type type, bool nonblocking>
	class tcp_listener;

	// IPv4 blocking: single thread, operates one at a time
	template<>
	class tcp_listener<sockaddr_type_in, false> : public tcp_socket<sockaddr_type_in, false> {
	protected:
		using socktype = tcp_socket<sockaddr_type_in, false>::socktype;
	public:
		explicit tcp_listener(int backlog = std::numeric_limits<int>::max()) : backlog(backlog), tcp_socket<sockaddr_type_in, false>() {
			if(!bind()) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to bind; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
				}
				throw tcp_bind_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
			}
		}
		explicit tcp_listener(in_port_t port, in_addr_t ip, int backlog = std::numeric_limits<int>::max()) : backlog(backlog), tcp_socket<sockaddr_type_in, false>(port, ip) {
			if(!bind()) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to bind; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
				}
				throw tcp_bind_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
			}
		}
		explicit tcp_listener(in_port_t port, std::string_view ip_str, int backlog = std::numeric_limits<int>::max()) : backlog(backlog), tcp_socket<sockaddr_type_in, false>(port, ip_str) {
			if(!bind()) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to bind; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
				}
				throw tcp_bind_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
			}
		}

		void loop(std::atomic_bool const &flag_quit) {
			while(!flag_quit.load(std::memory_order_seq_cst)) {
				if(!listen()) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to listen; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
					}
					throw tcp_listen_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
				}

				socktype peer_addr;
				int peer_sockfd = accept(peer_addr);
				if(peer_sockfd == -1) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to accept; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
					}
					throw tcp_accept_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
				}

				tcp_transceiver<sockaddr_type_in, false> transceiver(peer_sockfd, peer_addr.sin_port, peer_addr.sin_addr.s_addr);
				if(!transceive(flag_quit, transceiver)) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed while transceiving.\n");
					}
					throw tcp_transceive_failure_exception();
				}
			}
		}

	protected:
		virtual bool transceive(std::atomic_bool const &flag_quit, tcp_transceiver<sockaddr_type_in, false> const &transceiver) = 0;

	private:
		int backlog;
		[[nodiscard]] bool bind() const {
			return !::bind(this->sockfd, reinterpret_cast<sockaddr const*>(&this->addr), sizeof(this->addr));
		}

		[[nodiscard]] bool listen() const {
			return !::listen(this->sockfd, backlog);
		}

		[[nodiscard]] int accept(socktype &peer_addr) const {
			socklen_t peer_addrlen;
			return ::accept(this->sockfd, reinterpret_cast<sockaddr*>(&peer_addr), &peer_addrlen);
		}

		[[nodiscard]] int accept(socktype &peer_addr, socklen_t &peer_addrlen) const {
			return ::accept(this->sockfd, reinterpret_cast<sockaddr*>(&peer_addr), &peer_addrlen);
		}
	};

	// IPv4 nonblocking
	template<>
	class tcp_listener<sockaddr_type_in, true> : public tcp_socket<sockaddr_type_in, true> {
	protected:
		using socktype = tcp_socket<sockaddr_type_in, true>::socktype;
	public:
		explicit tcp_listener(int backlog = std::numeric_limits<int>::max()) : backlog(backlog), tcp_socket<sockaddr_type_in, true>() {
			if(!bind()) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to bind; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
				}
				throw tcp_bind_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
			}
		}
		explicit tcp_listener(in_port_t port, in_addr_t ip, int backlog = std::numeric_limits<int>::max()) : backlog(backlog), tcp_socket<sockaddr_type_in, true>(port, ip) {
			if(!bind()) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to bind; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
				}
				throw tcp_bind_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
			}
		}
		explicit tcp_listener(in_port_t port, std::string_view ip_str, int backlog = std::numeric_limits<int>::max()) : backlog(backlog), tcp_socket<sockaddr_type_in, true>(port, ip_str) {
			if(!bind()) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to bind; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
				}
				throw tcp_bind_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
			}
		}

		void loop(std::atomic_bool const &flag_quit) {
			while(!flag_quit.load(std::memory_order_seq_cst)) {
				if(!listen()) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to listen; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
					}
					throw tcp_listen_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
				}

				socktype peer_addr;
				int peer_sockfd = accept(peer_addr);
				if(peer_sockfd == -1) {
					if(errno != EAGAIN && errno != EWOULDBLOCK) continue;
					else {
						if constexpr(debug_mode) {
							::fprintf(stderr, "Failed to accept; port = %hu, ip = %u\n", this->addr.sin_port, this->addr.sin_addr.s_addr);
						}
						throw tcp_accept_failure_exception(this->addr.sin_port, this->addr.sin_addr.s_addr);
					}
				}

				// TODO: Fix [&].
				std::thread t([&]() {
					tcp_transceiver<sockaddr_type_in, true> transceiver(peer_sockfd, peer_addr.sin_port, peer_addr.sin_addr.s_addr);
					if(!transceive(flag_quit, transceiver)) {
						if constexpr(debug_mode) {
							::fprintf(stderr, "Failed while transceiving.\n");
						}
						throw tcp_transceive_failure_exception();
					}
				});
				t.detach();
			}
		}

	protected:
		virtual bool transceive(std::atomic_bool const &flag_quit, tcp_transceiver<sockaddr_type_in, true> const &transceiver) = 0;

	private:
		int backlog;
		[[nodiscard]] bool bind() const {
			return !::bind(this->sockfd, reinterpret_cast<sockaddr const*>(&this->addr), sizeof(this->addr));
		}

		[[nodiscard]] bool listen() const {
			return !::listen(this->sockfd, backlog);
		}

		[[nodiscard]] int accept(socktype &peer_addr) const {
			socklen_t peer_addrlen;
			return ::accept(this->sockfd, reinterpret_cast<sockaddr*>(&peer_addr), &peer_addrlen);
		}

		[[nodiscard]] int accept(socktype &peer_addr, socklen_t &peer_addrlen) const {
			return ::accept(this->sockfd, reinterpret_cast<sockaddr*>(&peer_addr), &peer_addrlen);
		}
	};
}

#endif

#include "include/socket/tcp_listener.hpp"

#include <atomic>
#include <iostream>
#include <thread>

namespace net = OFCT::networking;

class echo_server
: public net::tcp_listener<net::sockaddr_type_in, false> {
public:
	explicit echo_server(int backlog = std::numeric_limits<int>::max())
	  : net::tcp_listener<net::sockaddr_type_in, false>(backlog) {}

	explicit echo_server(in_port_t port, in_addr_t ip = INADDR_ANY, int backlog = std::numeric_limits<int>::max())
	  : net::tcp_listener<net::sockaddr_type_in, false>(port, ip, backlog) {}

	explicit echo_server(in_port_t port, std::string_view ip_str, int backlog = std::numeric_limits<int>::max())
	  : net::tcp_listener<net::sockaddr_type_in, false>(port, ip_str, backlog) {}

protected:
	virtual bool transceive(std::atomic_bool const &flag_quit, net::tcp_transceiver<net::sockaddr_type_in, false> const &transceiver) final {
		uint8_t buffer[1024]{};
		size_t offset = 0;
		while(offset < 1024) {
			size_t const size = 1024 - offset;
			ssize_t const received = transceiver.recv_raw(buffer, size);
			if(received == -1) return false;
			offset += received;
			if(offset && !buffer[offset - 1]) break;
		}

		size_t const len = offset;
		return transceiver.send(buffer, len);
	}

private:
};

int main() {
	echo_server server(9999, INADDR_ANY);
	std::atomic_bool flag_quit(false);
	std::thread thread([&server, &flag_quit]() {
		server.loop(flag_quit);
	});
	sleep(5);
	flag_quit.store(false, std::memory_order_seq_cst);
	thread.join();
}
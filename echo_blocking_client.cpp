#include "include/socket/tcp_client.hpp"

#include <iostream>

namespace net = OFCT::networking;

int main() {
	net::tcp_client<net::sockaddr_type_in, false> client(9999, "127.0.0.1");
	if(!client.connect()) {
		::printf("client: Failed to connect.\n");
		return 1;
	}
	std::string input;
	std::getline(std::cin, input);
	input = input.substr(0, 1023);
	if(!client.send(input)) {
		::printf("client: Failed to send.\n");
		return 1;
	}

	std::string received;
	if(!client.recv(received, input.size())) {
		::printf("client: Failed to recv.\n");
		return 1;
	}
	std::cout << received << '\n';
}
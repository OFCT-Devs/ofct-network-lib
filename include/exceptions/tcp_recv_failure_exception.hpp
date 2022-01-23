#ifndef SERVER_TCP_RECV_FAILURE_EXCEPTION_HPP
#define SERVER_TCP_RECV_FAILURE_EXCEPTION_HPP

#include <exception>

namespace OFCT::networking {
	class tcp_recv_failure_exception : public std::exception {
	public:
		[[nodiscard]] char const *what() const noexcept final {
			return "Failed to receive.";
		}
	};
}

#endif //SERVER_TCP_RECV_FAILURE_EXCEPTION_HPP

#ifndef SERVER_SOCKET_NONBLOCKING_SETUP_FAILED_EXCEPTION_HPP
#define SERVER_SOCKET_NONBLOCKING_SETUP_FAILED_EXCEPTION_HPP

#include <exception>

namespace OFCT::networking {
	class socket_nonblocking_setup_failed_exception : public std::exception {
	public:
		[[nodiscard]] char const *what() const noexcept final {
			return "Failed to set socket to nonblocking mode.";
		}
	};
}
#endif

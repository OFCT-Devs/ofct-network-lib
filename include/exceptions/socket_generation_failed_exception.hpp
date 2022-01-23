#ifndef SERVER_SOCKET_GENERATION_FAILED_EXCEPTION_HPP
#define SERVER_SOCKET_GENERATION_FAILED_EXCEPTION_HPP

#include <exception>

namespace OFCT::networking {
	class socket_generation_failed_exception : public std::exception {
	public:
		[[nodiscard]] char const *what() const noexcept final {
			return "Failed to generate socket.";
		}
	};
}

#endif

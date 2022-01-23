#ifndef SERVER_TCP_TRANSCEIVE_FAILURE_EXCEPTION_HPP
#define SERVER_TCP_TRANSCEIVE_FAILURE_EXCEPTION_HPP

#include <exception>

namespace OFCT::networking {
	class tcp_transceive_failure_exception : public std::exception {
	public:
		[[nodiscard]] char const *what() const noexcept final {
			return "Failed while transceiving.";
		}
	};
}

#endif

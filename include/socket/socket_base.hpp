#ifndef OFCT_NETWORK_socket_socket_base_hpp
#define OFCT_NETWORK_socket_socket_base_hpp

#include <fcntl.h>
#include <sys/socket.h>

#include <cassert>

// #include <debug/debug_mode.hpp>
#include "../debug/debug_mode.hpp"
#include "../exceptions/socket_exceptions.hpp"

namespace OFCT::networking {
    enum class sock_domain {
        NONE,
        UNIX,
        LOCAL,
        INET,
        AX25,
        IPX,
        APPLETALK,
        NETROM,
        BRIDGE,
        ATMPVC,
        X25,
        INET6,
        ROSE,
        DECNET,
        NETBEUI,
        SECURITY,
        KEY,
        NETLINK,
        PACKET,
        ECONET,
        ATMSVC,
        RDS,
        IRDA,
        PPPOX,
        WANPIPE,
        LLC,
        IB,
        MPLS,
        CAN,
        TIPC,
        BLUETOOTH,
        IUCV,
        RXRPC,
        ISDN,
        PHONET,
        IEEE802154,
        CAIF,
        ALG,
        VSOCK,
        KCM,
        QIPCRTR,
        SMC,
        XDP,
    };

    constexpr auto const domain_unix = sock_domain::UNIX;
    constexpr auto const domain_local = sock_domain::LOCAL;
    constexpr auto const domain_inet = sock_domain::INET;
    constexpr auto const domain_ax25 = sock_domain::AX25;
    constexpr auto const domain_ipx = sock_domain::IPX;
    constexpr auto const domain_appletalk = sock_domain::APPLETALK;
    constexpr auto const domain_x25 = sock_domain::X25;
    constexpr auto const domain_inet6 = sock_domain::INET6;
    constexpr auto const domain_decnet = sock_domain::DECNET;
    constexpr auto const domain_key = sock_domain::KEY;
    constexpr auto const domain_netlink = sock_domain::NETLINK;
    constexpr auto const domain_packet = sock_domain::PACKET;
    constexpr auto const domain_rds = sock_domain::RDS;
    constexpr auto const domain_pppox = sock_domain::PPPOX;
    constexpr auto const domain_llc = sock_domain::LLC;
    constexpr auto const domain_ib = sock_domain::IB;
    constexpr auto const domain_mpls = sock_domain::MPLS;
    constexpr auto const domain_can = sock_domain::CAN;
    constexpr auto const domain_tipc = sock_domain::TIPC;
    constexpr auto const domain_bluetooth = sock_domain::BLUETOOTH;
    constexpr auto const domain_alg = sock_domain::ALG;
    constexpr auto const domain_vsock = sock_domain::VSOCK;
    constexpr auto const domain_kcm = sock_domain::KCM;
    constexpr auto const domain_xdp = sock_domain::XDP;
    
    constexpr int sock_domain_to_AF(sock_domain domain) {
        switch(domain) {
        case domain_unix: return AF_UNIX;
        case domain_local: return AF_LOCAL;
        case domain_inet: return AF_INET;
        case domain_ax25: return AF_AX25;
        case domain_ipx: return AF_IPX;
        case domain_appletalk: return AF_APPLETALK;
        case domain_x25: return AF_X25;
        case domain_inet6: return AF_INET6;
        case domain_decnet: return AF_DECnet;
        case domain_key: return AF_KEY;
        case domain_netlink: return AF_NETLINK;
        case domain_packet: return AF_PACKET;
        case domain_rds: return AF_RDS;
        case domain_pppox: return AF_PPPOX;
        case domain_llc: return AF_LLC;
        case domain_ib: return AF_IB;
        case domain_mpls: return AF_MPLS;
        case domain_can: return AF_CAN;
        case domain_tipc: return AF_TIPC;
        case domain_bluetooth: return AF_BLUETOOTH;
        case domain_alg: return AF_ALG;
        case domain_vsock: return AF_VSOCK;
        case domain_kcm: return AF_KCM;
        case domain_xdp: return AF_XDP;
        default: return -1;
        }
    }

    enum class sock_type {
        NONE,
        STREAM,
        DGRAM,
        SEQPACKET,
        RAW,
        RDM,
        PACKET,
        /* TODO: NONBLOCK & CLOEXEC */
        NONBLOCK_STREAM,
        NONBLOCK_DGRAM,
        NONBLOCK_SEQPACKET,
        NONBLOCK_RAW,
        NONBLOCK_RDM,
        NONBLOCK_PACKET,
        CLOEXEC_STREAM,
        CLOEXEC_DGRAM,
        CLOEXEC_SEQPACKET,
        CLOEXEC_RAW,
        CLOEXEC_RDM,
        CLOEXEC_PACKET,
        NONBLOCK_CLOEXEC_STREAM,
        NONBLOCK_CLOEXEC_DGRAM,
        NONBLOCK_CLOEXEC_SEQPACKET,
        NONBLOCK_CLOEXEC_RAW,
        NONBLOCK_CLOEXEC_RDM,
        NONBLOCK_CLOEXEC_PACKET,
    };

    constexpr auto const type_stream = sock_type::STREAM;
    constexpr auto const type_dgram = sock_type::DGRAM;
    constexpr auto const type_seqpacket = sock_type::SEQPACKET;
    constexpr auto const type_raw = sock_type::RAW;
    constexpr auto const type_rdm = sock_type::RDM;
    constexpr auto const type_packet = sock_type::PACKET;

    constexpr int sock_type_to_SOCK(sock_type type) {
        switch(type) {
        case type_stream: return SOCK_STREAM;
        case type_dgram: return SOCK_DGRAM;
        case type_seqpacket: return SOCK_SEQPACKET;
        case type_raw: return SOCK_RAW;
        case type_rdm: return SOCK_RDM;
        case type_packet: return SOCK_PACKET;
        default: return -1;
        }
    }

    enum class sock_protocol { NONE, DEFAULT };

    constexpr auto const protocol_default = sock_protocol::DEFAULT;

    constexpr int sock_protocol_to_PROTOCOL(sock_protocol protocol) {
        switch(protocol) {
        case protocol_default: return 0;
        default: return -1;
        }
    }

    template<sock_domain domain, sock_type type, sock_protocol protocol, bool nonblocking>
    class socket_base {
    public:
        explicit socket_base() : sockfd(::socket(
                                sock_domain_to_AF(domain),
                                sock_type_to_SOCK(type),
                                sock_protocol_to_PROTOCOL(protocol))) {
	        if(sockfd == -1) {
		        if constexpr(debug_mode) {
			        ::fprintf(stderr, "Failed to generate socket.\n");
		        }
		        throw socket_generation_failed_exception();
	        }
	        if constexpr(nonblocking) {
				if(!set_nonblocking()) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to set socket to nonblocking mode.\n");
					}
					throw socket_nonblocking_setup_failed_exception();
				}
			}
        }

        explicit socket_base(int sockfd) : sockfd(sockfd) {
	        if(sockfd == -1) {
		        if constexpr(debug_mode) {
			        ::fprintf(stderr, "Failed to generate socket.\n");
		        }
		        throw socket_generation_failed_exception();
			}
	        if constexpr(nonblocking) {
				if(!set_nonblocking()) {
					if constexpr(debug_mode) {
						::fprintf(stderr, "Failed to set socket to nonblocking mode.\n");
					}
					throw socket_nonblocking_setup_failed_exception();
				}
			}
        }

		virtual ~socket_base() {
			assert(sockfd != -1);
			::close(sockfd);
		}

        [[nodiscard]] bool is_valid() const { return sockfd != -1; }

	protected:
		int sockfd;

    private:

	    [[nodiscard]] bool set_nonblocking() const {
		    int const flag = ::fcntl(sockfd, F_GETFL, 0);
		    if(::fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) == -1) {
				if constexpr(debug_mode) {
					::fprintf(stderr, "Failed to set socket %d to nonblocking mode: errno = %d\n", sockfd, errno);
				}
				return false;
			}
			return true;
		}
    };
}

#endif
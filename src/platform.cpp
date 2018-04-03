#include <remk/platform.hpp>

#ifndef _WIN32
#include <fcntl.h>
#endif

#include <assert.h>
#include <limits.h>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

namespace remk {
namespace platform {

void LoggerMixin::emit_log(int level, const std::stringstream &ss) noexcept {
    switch (level) {
    case REMK_PLATFORM_LOG_WARNING:
        std::clog << "[!] ";
        break;
    case REMK_PLATFORM_LOG_INFO:
        break;
    case REMK_PLATFORM_LOG_DEBUG:
        std::clog << "[D] ";
        break;
    default:
        std::clog << "[?] ";
        break;
    }
    std::clog << ss.str() << std::endl;
}

LoggerMixin::~LoggerMixin() noexcept {}

// TODO(bassosimone): see whether we can use a monotonic clock here.
int SystemMixin::timespec_get(timespec *ts, int base) noexcept {
#ifdef _WIN32
    return ::timespec_get(ts, base);
#else
    if (ts == nullptr || base != TIME_UTC) {
        errno = EINVAL;
        return 0;
    }
    timeval tv{};
    if (::gettimeofday(&tv, nullptr) != 0) {
        return 0;
    }
    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec * 1'000;
    return TIME_UTC;
#endif
}

int SystemMixin::get_last_error() noexcept {
#ifdef _WIN32
    return GetLastError();
#else
    return errno;
#endif
}

void SystemMixin::set_last_error(int error_code) noexcept {
#ifdef _WIN32
    SetLastError(error_code);
#else
    errno = error_code;
#endif
}

int SystemMixin::getaddrinfo(const char *hostname, const char *servname,
      const addrinfo *hints, addrinfo **res) noexcept {
    return ::getaddrinfo(hostname, servname, hints, res);
}

void SystemMixin::freeaddrinfo(addrinfo *aip) noexcept {
    return ::freeaddrinfo(aip);
}

Socket SystemMixin::socket(int domain, int type, int protocol) noexcept {
    return ::socket(domain, type, protocol);
}

int SystemMixin::connect(
      Socket handle, const sockaddr *saddr, Socklen salen) noexcept {
    return ::connect(handle, saddr, salen);
}

#ifdef _WIN32
int SystemMixin::system_recvfrom(SOCKET handle, char *buffer, int count,
      int flags, sockaddr *sa, int *len) noexcept {
    return ::recvfrom(handle, buffer, count, flags, sa, len);
}
#else
ssize_t SystemMixin::system_recvfrom(int handle, void *buffer, size_t count,
      int flags, sockaddr *sa, socklen_t *len) noexcept {
    return ::recvfrom(handle, buffer, count, flags, sa, len);
}
#endif

Ssize SystemMixin::recvfrom(Socket handle, void *buffer, Size count, int flags,
      sockaddr *sa, Socklen *len) noexcept {
#ifdef _WIN32
    if (count > INT_MAX) {
        WSASetLastError(WSAEINVAL);
        return -1;
    }
    return (Ssize)system_recvfrom(
          handle, (char *)buffer, (int)count, flags, sa, len);
#else
    return system_recvfrom(handle, buffer, count, flags, sa, len);
#endif
}

Ssize SystemMixin::recv(
      Socket handle, void *buffer, Size count, int flags) noexcept {
    return this->recvfrom(handle, buffer, count, flags, nullptr, nullptr);
}

#ifdef _WIN32
int SystemMixin::system_sendto(SOCKET handle, const char *buffer, int count,
      int flags, const sockaddr *sa, int len) noexcept {
    return ::sendto(handle, buffer, count, flags, sa, len);
}
#else
ssize_t SystemMixin::system_sendto(int handle, const void *buffer, size_t count,
      int flags, const sockaddr *sa, socklen_t len) noexcept {
    return ::sendto(handle, buffer, count, flags, sa, len);
}
#endif

Ssize SystemMixin::sendto(Socket handle, const void *buffer, Size count,
      int flags, const sockaddr *sa, Socklen len) noexcept {
#ifdef _WIN32
    if (count > INT_MAX) {
        WSASetLastError(WSAEINVAL);
        return -1;
    }
    return (Ssize)system_sendto(
          handle, (const char *)buffer, (int)count, flags, sa, len);
#else
    return system_sendto(handle, buffer, count, flags, sa, len);
#endif
}

Ssize SystemMixin::send(
      Socket handle, const void *buffer, Size count, int flags) noexcept {
    return this->sendto(handle, buffer, count, flags, nullptr, 0);
}

int SystemMixin::closesocket(Socket handle) noexcept {
#ifdef _WIN32
    return ::closesocket(handle);
#else
    return ::close(handle);
#endif
}

int SystemMixin::select(int maxfd, fd_set *readset, fd_set *writeset,
      fd_set *exceptset, timeval *timeout) noexcept {
    return ::select(maxfd, readset, writeset, exceptset, timeout);
}

#ifdef _WIN32
int SystemMixin::system_ioctlsocket(
      SOCKET s, long cmd, unsigned long *argp) noexcept {
    return ::ioctlsocket(s, cmd, argp);
}
#else
int SystemMixin::system_fcntl_int(int fd, int cmd, int arg) noexcept {
    return ::fcntl(fd, cmd, arg);
}
#endif

#ifdef _WIN32
int SystemMixin::system_wsasend(SOCKET s, LPWSABUF lpBuffers,
      DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
      LPWSAOVERLAPPED lpOverlapped,
      LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) noexcept {
    return ::WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags,
          lpOverlapped, lpCompletionRoutine);
}
#else
ssize_t SystemMixin::system_writev(
      int fd, const struct iovec *iov, int iovcnt) noexcept {
    return ::writev(fd, iov, iovcnt);
}
#endif

Ssize SystemMixin::writev(
      Socket socket, const iovec *iov, int iovcnt) noexcept {
#ifdef _WIN32
    if (iov == nullptr || iovcnt < 0 || iovcnt > IOV_MAX) {
        WSASetLastError(WSAEINVAL);
        return -1;
    }
    // To simplify reasoning about the code below in both Win32 and Win64, we
    // fail when requested to send more than `INT_MAX - 1` bytes.
    constexpr SIZE_T max_size = INT_MAX - 1;
    auto buffs = std::make_unique<WSABUF[]>(iovcnt);
    {
        auto iop = &iov[0];
        auto bufp = buffs.get();
        SIZE_T total = 0;
        for (auto i = 0; i < iovcnt; ++i, ++iop, ++bufp) {
            if (iop->iov_len > max_size || total > max_size - iop->iov_len) {
                WSASetLastError(WSAEINVAL);
                return -1;
            }
            total += iop->iov_len;
            bufp->len = (unsigned long)iop->iov_len;
            bufp->buf = (char *)iop->iov_base;
        }
        assert(total <= max_size);
    }
    DWORD nsent = 0;
    if (this->system_wsasend(socket, buffs.get(), (DWORD)iovcnt, &nsent, 0,
              nullptr, nullptr) != 0) {
        return -1;
    }
    assert(nsent <= max_size);
    return nsent;
#else
    return this->system_writev(socket, iov, iovcnt);
#endif
}

SystemMixin::~SystemMixin() noexcept {}

double Context::now() noexcept {
    timespec ts{};
    auto rv = this->timespec_get(&ts, TIME_UTC);
    if (rv != TIME_UTC) {
        return -1.0;
    }
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1'000'000'000;
}

int Context::setnonblocking(Socket sock, bool enable) noexcept {
#ifdef _WIN32
    unsigned long ul_enable = enable;
    if (this->system_ioctlsocket(sock, FIONBIO, &ul_enable) != 0) {
        return -1;
    }
#else
    auto flags = this->system_fcntl_int(sock, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }
    if (enable) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    if (this->system_fcntl_int(sock, F_SETFL, flags) != 0) {
        return -1;
    }
#endif
    return 0;
}

int Context::sockaddr_ntop(
      const sockaddr *sa, std::string *address, std::string *port) noexcept {
    if (sa == nullptr || address == nullptr || port == nullptr) {
        this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
        return -1;
    }
    Socklen len = 0;
    switch (sa->sa_family) {
    case AF_INET:
        len = sizeof(sockaddr_in);
        break;
    case AF_INET6:
        len = sizeof(sockaddr_in6);
        break;
    default:
        this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
        return -1;
    }
    int flags = NI_NUMERICHOST | NI_NUMERICSERV;
    char a[NI_MAXHOST], p[NI_MAXSERV];
    if (::getnameinfo(sa, len, a, sizeof(a), p, sizeof(p), flags) != 0) {
        this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
        return -1;
    }
    *address = a;
    *port = p;
    return 0;
}

int Context::sockaddr_pton(
      const char *address, const char *port, sockaddr_storage *sst) noexcept {
    if (address == nullptr || port == nullptr || sst == nullptr) {
        this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
        return -1;
    }
    memset(sst, 0, sizeof(*sst));
    addrinfo *rp = nullptr;
    addrinfo hints{};
    hints.ai_flags |= AI_NUMERICHOST | AI_NUMERICSERV;
    // Note: here I'm using ::getaddrinfo() and ::freeaddrinfo() intentionally
    if (::getaddrinfo(address, port, &hints, &rp) != 0) {
        this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
        return -1;
    }
    assert(rp != nullptr && rp->ai_addr && rp->ai_addrlen <= sizeof(*sst));
    memcpy(sst, rp->ai_addr, rp->ai_addrlen);
    ::freeaddrinfo(rp);
    return 0;
}

std::string Context::hexdump(const void *data, size_t count) noexcept {
    std::stringstream ss;
    if (data != nullptr) {
        const unsigned char *cbase = (const unsigned char *)data;
        for (size_t i = 0; i < count; ++i) {
            ss << std::hex << std::setfill('0') << std::setw(2)
               << (unsigned int)cbase[i];
            if (i < count - 1) { // safe b/c min(count) == 1 in this loop
                ss << " ";
            }
        }
    }
    return ss.str();
}

int Context::wsainit() noexcept {
#ifdef _WIN32
    WORD version = MAKEWORD(2, 2);
    WSADATA wsadata;
    if (WSAStartup(version, &wsadata) != 0) {
        return -1;
    }
#endif
    return 0;
}

Context::~Context() noexcept {}

DeferClosesocket::DeferClosesocket(Context *ctx, Socket sock) noexcept
    : ctx_{ctx}, sock_{sock} {}

DeferClosesocket::~DeferClosesocket() noexcept {
    if (sock_ != -1) {
        (void)ctx_->closesocket(sock_);
    }
}

DeferFreeaddrinfo::DeferFreeaddrinfo(Context *ctx, addrinfo *aip) noexcept
    : ctx_{ctx}, aip_{aip} {}

DeferFreeaddrinfo::~DeferFreeaddrinfo() noexcept {
    if (aip_ != nullptr) {
        (void)ctx_->freeaddrinfo(aip_);
    }
}

} // namespace platform
} // namespace remk

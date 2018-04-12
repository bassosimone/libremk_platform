#include <remk/platform/platform.hpp>

#ifndef _WIN32
#include <fcntl.h>
#endif

#include <assert.h>
#include <limits.h>
#include <string.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "src/strcasecmp.h"
#include "src/strtonum.h"
#include "src/utf8_decode.h"

namespace remk {
namespace platform {

void SettingsMixin::set_value_double(const char *name, double value) noexcept {
    if (name != nullptr) {
        SettingsValue v;
        v.v = std::to_string(value);
        v.t = SettingsType::DOUBLE_SETTING;
        values_[name] = std::move(v);
    }
}

bool SettingsMixin::get_value_double(const char *name, double *value) noexcept {
    if (name == nullptr || value == nullptr || values_.count(name) <= 0) {
        return false;
    }
    auto &v = values_.at(name);
    if (v.t != SettingsType::DOUBLE_SETTING) {
        return false;
    }
    std::stringstream ss;
    ss << v.v;
    ss >> *value;
    if (!ss.eof() || ss.bad()) {
        return false;
    }
    return true;
}

void SettingsMixin::set_value_int(const char *name, int64_t value) noexcept {
    if (name != nullptr) {
        SettingsValue v;
        v.v = std::to_string(value);
        v.t = SettingsType::INT_SETTING;
        values_[name] = std::move(v);
    }
}

bool SettingsMixin::get_value_int(const char *name, int64_t *value) noexcept {
    if (name == nullptr || value == nullptr || values_.count(name) <= 0) {
        return false;
    }
    auto &v = values_.at(name);
    if (v.t != SettingsType::INT_SETTING) {
        return false;
    }
    std::stringstream ss;
    ss << v.v;
    ss >> *value;
    if (!ss.eof() || ss.bad()) {
        return false;
    }
    return true;
}

void SettingsMixin::set_value_string(
      const char *name, std::string value) noexcept {
    if (name != nullptr) {
        SettingsValue v;
        std::swap(v.v, value);
        v.t = SettingsType::STRING_SETTING;
        values_[name] = std::move(v);
    }
}

bool SettingsMixin::get_value_string(
      const char *name, std::string *value) noexcept {
    if (name == nullptr || value == nullptr || values_.count(name) <= 0) {
        return false;
    }
    auto &v = values_.at(name);
    if (v.t != SettingsType::STRING_SETTING) {
        return false;
    }
    *value = v.v;
    return true;
}

EventValue::~EventValue() noexcept {}
LogEventValue::~LogEventValue() noexcept {}

void LoggerAndEmitterMixin::emit_event(Event ev) noexcept {
    if (ev.name != REMK_LOG_EVENT_NAME) {
        std::clog << "[!] unsupported event: " << ev.name << std::endl;
        return;
    }
    // Note: failure to perform the cast implies a programmer error
    auto value = std::dynamic_pointer_cast<LogEventValue>(std::move(ev.value));
    if (!value) {
        assert(false);
        abort();
        // NOTREACHED
    }
    switch (value->level) {
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
    std::clog << value->message << std::endl;
}

void LoggerAndEmitterMixin::emit_log(
      int level, const std::stringstream &ss) noexcept {
    Event ev;
    ev.name = REMK_LOG_EVENT_NAME;
    auto value = std::make_shared<LogEventValue>();
    value->message = ss.str();
    value->level = level;
    ev.value = std::move(value);
    assert(!!ev.value);
    emit_event(std::move(ev));
}

LoggerAndEmitterMixin::~LoggerAndEmitterMixin() noexcept {}

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

long long SystemMixin::strtonum(const char *numstr, long long minval,
      long long maxval, const char **errstrp) noexcept {
    return ::strtonum(numstr, minval, maxval, errstrp);
}

int SystemMixin::strcasecmp(const char *s1, const char *s2) noexcept {
    return ::strcasecmp(s1, s2);
}

int SystemMixin::strncasecmp(
      const char *s1, const char *s2, size_t n) noexcept {
    return ::strncasecmp(s1, s2, n);
}

SystemMixin::~SystemMixin() noexcept {}

double Context::steady_clock_now() noexcept {
    auto now = std::chrono::steady_clock::now();
    auto usec = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    return usec.time_since_epoch().count() / (double)1'000'000;
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

Socket Context::connect_tcp(const char *hostname, const char *port) noexcept {
    if (hostname == nullptr || port == nullptr) {
        this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
        return -1;
    }
    addrinfo hints{};
    hints.ai_flags |= AI_NUMERICHOST | AI_NUMERICSERV;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *rp = nullptr;
    int rv = this->getaddrinfo(hostname, port, &hints, &rp);
    if (rv != 0) {
        hints.ai_flags &= ~AI_NUMERICHOST;
        rv = this->getaddrinfo(hostname, port, &hints, &rp);
        if (rv != 0) {
            REMK_PLATFORM_WARN(this, "getaddrinfo: " << rv);
            return -1;
        }
    }
    remk::platform::DeferFreeaddrinfo dfa{this, rp};
    REMK_PLATFORM_DEBUG(this, "getaddrinfo: success");
    for (auto ai = rp; ai != nullptr; ai = ai->ai_next) {
        auto sock = this->socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock == -1) {
            REMK_PLATFORM_WARN(this, "socket");
            continue;
        }
        REMK_PLATFORM_DEBUG(this, "socket: success");
        if (this->connect(sock, ai->ai_addr,
                  (remk::platform::Socklen)ai->ai_addrlen) == 0) {
            REMK_PLATFORM_DEBUG(this, "connect: success");
            return sock;
        }
        REMK_PLATFORM_WARN(this, "connect");
        this->closesocket(sock);
    }
    REMK_PLATFORM_WARN(this, "all connect attempts failed");
    /* The `errno` value should be the last error that occurred. */
    return -1;
}

Ssize Context::readn(
      Socket handle, void *buffer, Size count, int flags) noexcept {
    Ssize retval = 0;
    Size off = 0;
    while (off < count) {
        // Be reactive so it works with blocking sockets as well.
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(handle, &readset);
        int ctrl = this->select( //
              handle + 1, &readset, nullptr, nullptr, nullptr);
        if (ctrl == -1) {
#ifndef _WIN32
            if (errno == EINTR) {
                continue;
            }
#endif
            return -1;
        }
        assert(ctrl > 0);
        // Cast to `char *` required to force pointer arithmetics.
        Ssize n = this->recv(handle, (char *)buffer + off, count - off, flags);
        if (n <= 0) {
            return n; // Either all success or failure.
        }
        // Make sure `retval += n` won't overflow `INT_MAX` (arbitrary).
        if (n > INT_MAX || retval > INT_MAX - n) {
            this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
            return -1;
        }
        retval += n;
        off += (Size)n;
    }
    return retval;
}

Ssize Context::writen(
      Socket handle, const void *buffer, Size count, int flags) noexcept {
    Ssize retval = 0;
    Size off = 0;
    while (off < count) {
        // Be reactive so it works with blocking sockets as well.
        fd_set writeset;
        FD_ZERO(&writeset);
        FD_SET(handle, &writeset);
        int ctrl = this->select( //
              handle + 1, nullptr, &writeset, nullptr, nullptr);
        if (ctrl == -1) {
#ifndef _WIN32
            if (errno == EINTR) {
                continue;
            }
#endif
            return -1;
        }
        assert(ctrl > 0);
        // Cast to `char *` required to force pointer arithmetics.
        Ssize n = this->send(handle, (char *)buffer + off, count - off, flags);
        if (n <= 0) {
            return n; // Either all success or failure.
        }
        // Make sure `retval += n` won't overflow `INT_MAX` (arbitrary).
        if (n > INT_MAX || retval > INT_MAX - n) {
            this->set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
            return -1;
        }
        retval += n;
        off += (Size)n;
    }
    return retval;
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

/*static*/ std::string Context::classify_system_error(
      int system_error) noexcept {
#if !defined _WIN32 && EWOUDLBLOCK != EAGAIN // Theoretically possible
    if (system_error == EAGAIN) {
        system_error = EWOULDBLOCK;
    }
#endif
    switch (system_error) {
    case REMK_PLATFORM_ERROR_NAME(ALREADY):
        return "connection_already_in_progress";
    case REMK_PLATFORM_ERROR_NAME(CONNABORTED):
        return "connection_aborted";
    case REMK_PLATFORM_ERROR_NAME(CONNREFUSED):
        return "connection_refused";
    case REMK_PLATFORM_ERROR_NAME(CONNRESET):
        return "connection_reset";
    case REMK_PLATFORM_ERROR_NAME(HOSTUNREACH):
        return "host_unreachable";
    case REMK_PLATFORM_ERROR_NAME(INPROGRESS):
        return "operation_in_progress";
    case REMK_PLATFORM_ERROR_NAME(INTR):
        return "interrupted";
    case REMK_PLATFORM_ERROR_NAME(INVAL):
        return "invalid_argument";
    case REMK_PLATFORM_ERROR_NAME(NETDOWN):
        return "network_down";
    case REMK_PLATFORM_ERROR_NAME(NETRESET):
        return "network_reset";
    case REMK_PLATFORM_ERROR_NAME(NETUNREACH):
        return "network_unreachable";
    case REMK_PLATFORM_ERROR_NAME(NOBUFS):
        return "no_buffer_space";
#if !defined _WIN32 // ENOMEM, EPIPE not be available on Windows
    case ENOMEM:
        return "not_enough_memory";
    case EPIPE:
        return "broken_pipe";
#endif
    case REMK_PLATFORM_ERROR_NAME(TIMEDOUT):
        return "timed_out";
    case REMK_PLATFORM_ERROR_NAME(WOULDBLOCK):
        return "operation_would_block";
    }
    std::stringstream ss;
    ss << "unknown_system_error " << system_error;
    return ss.str();
}

/*static*/ bool Context::is_valid_utf8(const std::string &str) noexcept {
    uint32_t ignored = 0;
    uint32_t state = UTF8_ACCEPT;
    for (const char &ch : str) {
        // Implementation note: rather than casting to `uint32_t` directly just
        // make `ch` unsigned and let the compiler extend the value. Here I am
        // trying to protect against the possibility that converting a negative
        // char value to `uint32_t` in some platforms may lead to obtaining an
        // unreasonably large value because of the leading `1`. I seem to recall
        // something like that happened to me in 2006 with a PPC, but too much
        // time has passed. If I'm wrong we can change this in the future.
        (void)remk_platform_utf8_decode(&state, &ignored, (unsigned char)ch);
        if (state == UTF8_REJECT) {
            return false;
        }
    }
    return state == UTF8_ACCEPT;
}

DeferClosesocket::DeferClosesocket(Context *ctx, Socket sock) noexcept
    : ctx_{ctx}, sock_{sock} {}

DeferClosesocket::~DeferClosesocket() noexcept {
    if (ctx_ != nullptr && sock_ != -1) {
        (void)ctx_->closesocket(sock_);
    }
}

DeferFreeaddrinfo::DeferFreeaddrinfo(Context *ctx, addrinfo *aip) noexcept
    : ctx_{ctx}, aip_{aip} {}

DeferFreeaddrinfo::~DeferFreeaddrinfo() noexcept {
    if (ctx_ != nullptr && aip_ != nullptr) {
        (void)ctx_->freeaddrinfo(aip_);
    }
}

} // namespace platform
} // namespace remk

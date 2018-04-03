#ifndef REMK_PLATFORM_HPP
#define REMK_PLATFORM_HPP

#ifdef _WIN32
#include <basetsd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <errno.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#endif

#include <time.h>

#include <sstream>
#include <string>

#ifndef TIME_UTC
#define TIME_UTC 1
#endif

#ifdef _WIN32
struct iovec {
    void *iov_base;
    SIZE_T iov_len;
};
#define IOV_MAX 64
#endif

#define REMK_PLATFORM_LOG_WARNING 0
#define REMK_PLATFORM_LOG_INFO 1
#define REMK_PLATFORM_LOG_DEBUG 2

#ifdef _WIN32
#define REMK_PLATFORM_ERROR_NAME(name_) WSAE##name_
#else
#define REMK_PLATFORM_ERROR_NAME(name_) E##name_
#endif

namespace remk {
namespace platform {

#ifdef _WIN32
using Socket = SOCKET;
using Size = SIZE_T;
using Socklen = int;
using Ssize = SSIZE_T;
#else
using Socket = int;
using Size = size_t;
using Socklen = socklen_t;
using Ssize = ssize_t;
#endif

class LoggerMixin {
  public:
    virtual void emit_log(int level, const std::stringstream &ss) noexcept;

    int get_log_level() noexcept { return level_; }

    void set_log_level(int level) noexcept { level_ = level; }

    virtual ~LoggerMixin() noexcept;

  private:
    int level_ = REMK_PLATFORM_LOG_DEBUG;
};

class SystemMixin {
  public:
    virtual int timespec_get(timespec *ts, int base) noexcept;

    virtual int get_last_error() noexcept;

    virtual void set_last_error(int error_code) noexcept;

    virtual int getaddrinfo(const char *hostname, const char *servname,
          const addrinfo *hints, addrinfo **res) noexcept;

    virtual void freeaddrinfo(addrinfo *aip) noexcept;

    virtual Socket socket(int domain, int type, int protocol) noexcept;

    virtual int connect(
          Socket handle, const sockaddr *saddr, Socklen len) noexcept;

    /* The following function has the same ABI of the corresponding system API,
       instead remk_platform_recv() has a uniform ABI regardless of the OS. */
#ifdef _WIN32
    virtual int system_recvfrom(SOCKET handle, char *buffer, int count,
          int flags, sockaddr *addr, int *len) noexcept;
#else
    virtual ssize_t system_recvfrom(int handle, void *buffer, size_t count,
          int flags, sockaddr *addr, socklen_t *len) noexcept;
#endif

    virtual Ssize recvfrom(Socket handle, void *buffer, Size count, int flags,
          sockaddr *addr, Socklen *len) noexcept;

    virtual Ssize recv(
          Socket handle, void *buffer, Size count, int flags) noexcept;

#ifdef _WIN32
    virtual int system_sendto(SOCKET handle, const char *buffer, int count,
          int flags, const sockaddr *addr, int len) noexcept;
#else
    virtual ssize_t system_sendto(int handle, const void *buffer, size_t count,
          int flags, const sockaddr *addr, socklen_t len) noexcept;
#endif

    virtual Ssize sendto(Socket handle, const void *buffer, Size count,
          int flags, const sockaddr *addr, Socklen len) noexcept;

    virtual Ssize send(
          Socket handle, const void *buffer, Size count, int flags) noexcept;

    virtual int closesocket(Socket handle) noexcept;

    virtual int select(int maxfd, fd_set *readset, fd_set *writeset,
          fd_set *exceptset, timeval *timeout) noexcept;

#ifdef _WIN32
    virtual int system_ioctlsocket(
          SOCKET s, long cmd, unsigned long *argp) noexcept;
#else
    virtual int system_fcntl_int(int fd, int cmd, int arg) noexcept;
#endif

#ifdef _WIN32
    virtual int system_wsasend(SOCKET s, LPWSABUF lpBuffers,
          DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
          LPWSAOVERLAPPED lpOverlapped,
          LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) noexcept;
#else
    virtual ssize_t system_writev(
          int fd, const struct iovec *iov, int iovcnt) noexcept;
#endif

    virtual Ssize writev(Socket socket, const iovec *iov, int iovcnt) noexcept;

    virtual ~SystemMixin() noexcept;
};

class Context : public LoggerMixin, public SystemMixin {
  public:
    virtual double now() noexcept;

    virtual int setnonblocking(Socket handle, bool enable) noexcept;

    virtual int sockaddr_pton(const char *address, const char *port,
          sockaddr_storage *sst) noexcept;

    virtual int sockaddr_ntop(
          const sockaddr *sa, std::string *address, std::string *port) noexcept;

    virtual std::string hexdump(const void *data, size_t count) noexcept;

    virtual Socket connect_tcp(const char *hostname, const char *port) noexcept;

    /* Continue reading until |count| bytes have been read. Internally uses
       this->select() so you can abort the read loop. */
    virtual Ssize readn(
          Socket handle, void *buffer, Size count, int flags) noexcept;

    /* Same as readn() but for writing. */
    virtual Ssize writen(
          Socket handle, const void *buffer, Size count, int flags) noexcept;

    /* Initializes Windows sockets. Is a no-op on Unix. Must be called
       from the main application, not from a DLL. */
    virtual int wsainit() noexcept;

    virtual ~Context() noexcept;
};

class DeferClosesocket {
  public:
    DeferClosesocket(Context *ctx, Socket sock) noexcept;
    ~DeferClosesocket() noexcept;

    DeferClosesocket(const DeferClosesocket &) = delete;
    DeferClosesocket &operator=(const DeferClosesocket &) = delete;
    DeferClosesocket(DeferClosesocket &&) = delete;
    DeferClosesocket &operator=(DeferClosesocket &&) = delete;

  private:
    Context *ctx_;
    Socket sock_;
};

class DeferFreeaddrinfo {
  public:
    DeferFreeaddrinfo(Context *ctx, addrinfo *aip) noexcept;
    ~DeferFreeaddrinfo() noexcept;

    DeferFreeaddrinfo(const DeferFreeaddrinfo &) = delete;
    DeferFreeaddrinfo &operator=(const DeferFreeaddrinfo &) = delete;
    DeferFreeaddrinfo(DeferFreeaddrinfo &&) = delete;
    DeferFreeaddrinfo &operator=(DeferFreeaddrinfo &&) = delete;

  private:
    Context *ctx_;
    addrinfo *aip_;
};

} // namespace platform
} // namespace remk

#define REMK_PLATFORM_EMIT_LOG_(ctx_, level_, statements_, print_last_error_)  \
    do {                                                                       \
        if (level_ <= ctx_->get_log_level()) {                                 \
            std::stringstream ss;                                              \
            ss << statements_;                                                 \
            if (print_last_error_) {                                           \
                ss << ": " << ctx_->get_last_error();                          \
            }                                                                  \
            ctx_->emit_log(level_, ss);                                        \
        }                                                                      \
    } while (0)

#define REMK_PLATFORM_WARN(ctx_, statements_)                                  \
    REMK_PLATFORM_EMIT_LOG_(ctx_, REMK_PLATFORM_LOG_WARNING, statements_, true)

#define REMK_PLATFORM_WARNX(ctx_, statements_)                                 \
    REMK_PLATFORM_EMIT_LOG_(ctx_, REMK_PLATFORM_LOG_WARNING, statements_, false)

#define REMK_PLATFORM_INFO(ctx_, statements_)                                  \
    REMK_PLATFORM_EMIT_LOG_(ctx_, REMK_PLATFORM_LOG_INFO, statements_, true)

#define REMK_PLATFORM_INFOX(ctx_, statements_)                                 \
    REMK_PLATFORM_EMIT_LOG_(ctx_, REMK_PLATFORM_LOG_INFO, statements_, false)

#define REMK_PLATFORM_DEBUG(ctx_, statements_)                                 \
    REMK_PLATFORM_EMIT_LOG_(ctx_, REMK_PLATFORM_LOG_DEBUG, statements_, true)

#define REMK_PLATFORM_DEBUGX(ctx_, statements_)                                \
    REMK_PLATFORM_EMIT_LOG_(ctx_, REMK_PLATFORM_LOG_DEBUG, statements_, false)

#endif
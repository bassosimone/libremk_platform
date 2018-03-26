#ifndef REMK_PLATFORM_CONTEXT_H
#define REMK_PLATFORM_CONTEXT_H
#ifdef __cplusplus

#include <remk/platform/aaa_base.h>

#include <sstream>

#define REMK_PLATFORM_LOG_WARNING 0
#define REMK_PLATFORM_LOG_INFO 1
#define REMK_PLATFORM_LOG_DEBUG 2

namespace remk {
namespace platform {

using Socket = remk_platform_socket_t;
using Socklen = remk_platform_socklen_t;
using Ssize = remk_platform_ssize_t;
using Size = remk_platform_size_t;

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

    virtual Socket socket(
          int domain, int type, int protocol) noexcept;

    virtual int connect(Socket handle, const sockaddr *saddr,
          Socklen len) noexcept;

#ifdef _WIN32
    virtual int system_recvfrom(SOCKET handle, char *buffer, int count,
          int flags, sockaddr *addr, int *len) noexcept;
#else
    virtual ssize_t system_recvfrom(int handle, void *buffer, size_t count,
          int flags, sockaddr *addr, socklen_t *len) noexcept;
#endif

    virtual Ssize recvfrom(Socket handle,
          void *buffer, Size count, int flags, sockaddr *addr,
          Socklen *len) noexcept;

#ifdef _WIN32
    virtual int system_sendto(SOCKET handle, const char *buffer, int count,
          int flags, const sockaddr *addr, int len) noexcept;
#else
    virtual ssize_t system_sendto(int handle, const void *buffer, size_t count,
          int flags, const sockaddr *addr, socklen_t len) noexcept;
#endif

    virtual Ssize sendto(Socket handle,
          const void *buffer, Size count, int flags,
          const sockaddr *addr, Socklen len) noexcept;

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

    virtual ~SystemMixin() noexcept;
};

class Context : public LoggerMixin, public SystemMixin {
  public:
    static void set_thread_local(Context *ctx) noexcept;

    static Context *get_thread_local() noexcept;

    virtual ~Context() noexcept;
};

} // namespace platform
} // namespace remk
#endif

#define REMK_PLATFORM_EMIT_LOG_(level_, statements_, print_last_error_)        \
    do {                                                                       \
        auto ctx = remk::platform::Context::get_thread_local();                \
        if (level_ <= ctx->get_log_level()) {                                  \
            std::stringstream ss;                                              \
            ss << statements_;                                                 \
            if (print_last_error_) {                                           \
                ss << ": " << ctx->get_last_error();                           \
            }                                                                  \
            ctx->emit_log(level_, ss);                                         \
        }                                                                      \
    } while (0)

#define REMK_PLATFORM_WARN(statements_)                                        \
    REMK_PLATFORM_EMIT_LOG_(REMK_PLATFORM_LOG_WARNING, statements_, true)

#define REMK_PLATFORM_WARNX(statements_)                                       \
    REMK_PLATFORM_EMIT_LOG_(REMK_PLATFORM_LOG_WARNING, statements_, false)

#define REMK_PLATFORM_INFO(statements_)                                        \
    REMK_PLATFORM_EMIT_LOG_(REMK_PLATFORM_LOG_INFO, statements_, true)

#define REMK_PLATFORM_INFOX(statements_)                                       \
    REMK_PLATFORM_EMIT_LOG_(REMK_PLATFORM_LOG_INFO, statements_, false)

#define REMK_PLATFORM_DEBUG(statements_)                                       \
    REMK_PLATFORM_EMIT_LOG_(REMK_PLATFORM_LOG_DEBUG, statements_, true)

#define REMK_PLATFORM_DEBUGX(statements_)                                      \
    REMK_PLATFORM_EMIT_LOG_(REMK_PLATFORM_LOG_DEBUG, statements_, false)

#endif

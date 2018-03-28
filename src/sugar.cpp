#include <remk/platform/sugar.h>

#include <remk/platform/context.h>

using namespace remk::platform;

double remk_platform_now() noexcept {
    return Context::get_thread_local()->now();
}

int remk_platform_setnonblocking(
      remk_platform_socket_t sock, int enable) noexcept {
    return Context::get_thread_local()->setnonblocking(sock, enable);
}

int remk_platform_sockaddr_ntop(
      const sockaddr *sa, std::string *address, std::string *port) noexcept {
    return Context::get_thread_local()->sockaddr_ntop(sa, address, port);
}

int remk_platform_sockaddr_pton(
      const char *address, const char *port, sockaddr_storage *sst) noexcept {
    return Context::get_thread_local()->sockaddr_pton(address, port, sst);
}

std::string remk_platform_hexdump(const void *data, size_t count) noexcept {
    return Context::get_thread_local()->hexdump(data, count);
}

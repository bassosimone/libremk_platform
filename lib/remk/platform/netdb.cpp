#include <remk/platform/netdb.h>
#include <remk/platform/context.hpp>

using namespace remk::platform;

int remk_platform_getaddrinfo(const char *hostname, const char *servname,
      const addrinfo *hints, addrinfo **res) noexcept {
  return Context::get_thread_local()->getaddrinfo(
      hostname, servname, hints, res);
}

void remk_platform_freeaddrinfo(addrinfo *aip) noexcept {
  Context::get_thread_local()->freeaddrinfo(aip);
}

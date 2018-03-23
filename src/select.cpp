#include <remk/platform/context.hpp>
#include <remk/platform/select.h>

using namespace remk::platform;

int remk_platform_select(int maxfd, fd_set *readset, fd_set *writeset,
                         fd_set *exceptset, timeval *timeout) noexcept {
  return Context::get_thread_local()->select(maxfd, readset, writeset,
                                             exceptset, timeout);
}

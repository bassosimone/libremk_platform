#include <remk/platform/time.h>
#include <remk/platform/context.hpp>

using namespace remk::platform;

int remk_platform_timespec_get(timespec *ts, int base) noexcept {
  return Context::get_thread_local()->timespec_get(ts, base);
}

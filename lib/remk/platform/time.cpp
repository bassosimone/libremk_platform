#include <remk/platform/time.h>
#include <remk/platform/context.hpp>

using namespace remk::platform;

int remk_platform_gettimeofday(timeval *tv, struct timezone *tz) noexcept {
  return Context::get_thread_local()->gettimeofday(tv, tz);
}

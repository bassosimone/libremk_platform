#include <remk/platform/sugar.h>
#include <remk/platform/time.h>

double remk_platform_now() noexcept {
  timespec ts{};
  auto rv = remk_platform_timespec_get(&ts, TIME_UTC);
  if (rv != TIME_UTC) {
    return -1.0;
  }
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1'000'000'000;
}

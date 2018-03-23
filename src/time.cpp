#include <remk/platform/context.h>
#include <remk/platform/time.h>

using namespace remk::platform;

int remk_platform_timespec_get(timespec *ts, int base) noexcept {
    return Context::get_thread_local()->timespec_get(ts, base);
}

#include <remk/platform/context.h>
#include <remk/platform/errno.h>

using namespace remk::platform;

int remk_platform_get_last_error() noexcept {
  return Context::get_thread_local()->get_last_error();
}

void remk_platform_set_last_error(int error_code) noexcept {
  Context::get_thread_local()->set_last_error(error_code);
}

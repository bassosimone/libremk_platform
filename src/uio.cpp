#include <remk/platform/uio.h>
#include <remk/platform/context.h>

remk_platform_ssize_t remk_platform_writev(
      remk_platform_socket_t socket, const iovec *iov, int iovcnt) noexcept {
    return remk::platform::Context::get_thread_local()->writev(
          socket, iov, iovcnt);

}

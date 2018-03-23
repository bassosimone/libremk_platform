#include <remk/platform/uio.h>

#include <assert.h>

#include <remk/platform/context.h>

remk_platform_ssize_t remk_platform_writev(
      remk_platform_socket_t socket, const iovec *iov, int iovcnt) noexcept {
    auto ctx = remk::platform::Context::get_thread_local();
#ifdef _WIN32
    if (iov == nullptr || iovcnt < 0 || iovcnt > IOV_MAX) {
        WSASetLastError(WSAEINVAL);
        return -1;
    }
    // To simplify reasoning about the code below in both Win32 and Win64, we
    // fail when requested to send more than `INT_MAX - 1` bytes.
    constexpr auto max_size = INT_MAX - 1;
    WSABUF buffs[IOV_MAX];
    remk_platform_size_t total = 0;
    for (int i = 0; i < iovcnt; ++i) {
        auto iop = &iov[i];
        if (iop->iov_len > max_size || total > max_size - iop->iov_len) {
            WSASetLastError(WSAEINVAL);
            return -1;
        }
        total += iop->iov_len;
        buffs[i].len = (unsigned long)iop->iov_len;
        buffs[i].buf = (char *)iop->iov_base;
    }
    assert(total <= max_size);
    DWORD nsent = 0;
    if (ctx->system_wsasend(socket, buffs, (DWORD)iovcnt, &nsent, 0, nullptr,
              nullptr) != 0) {
        return -1;
    }
    assert(nsent <= max_size);
    return (remk_platform_ssize_t)nsent;
#else
    return ctx->system_writev(socket, iov, iovcnt);
#endif
}

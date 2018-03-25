#include <remk/platform/uio.h>

#ifdef _WIN32
#include <assert.h>
#include <limits.h>
#include <memory>
#endif

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
    constexpr SIZE_T max_size = INT_MAX - 1;
    auto buffs = std::make_unique<WSABUF[]>(iovcnt);
    {
        auto iop = &iov[0];
        auto bufp = buffs.get();
        SIZE_T total = 0;
        for (auto i = 0; i < iovcnt; ++i, ++iop, ++bufp) {
            if (iop->iov_len > max_size || total > max_size - iop->iov_len) {
                WSASetLastError(WSAEINVAL);
                return -1;
            }
            total += iop->iov_len;
            bufp->len = (unsigned long)iop->iov_len;
            bufp->buf = (char *)iop->iov_base;
        }
        assert(total <= max_size);
    }
    DWORD nsent = 0;
    if (ctx->system_wsasend(socket, buffs.get(), (DWORD)iovcnt, &nsent, 0,
              nullptr, nullptr) != 0) {
        return -1;
    }
    assert(nsent <= max_size);
    return nsent;
#else
    return ctx->system_writev(socket, iov, iovcnt);
#endif
}

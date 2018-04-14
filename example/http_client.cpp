#include <remk/platform/platform.hpp>

#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>

int main() {
    auto ctx = std::make_unique<remk::platform::Context>();
    ctx->set_log_level(REMK_PLATFORM_LOG_DEBUG);
    if (ctx->wsainit() != 0) {
        REMK_PLATFORM_WARN(ctx, "wsainit");
        exit(EXIT_FAILURE);
    }
    auto begin = ctx->steady_clock_now();
    auto sock = ctx->connect_tcp("www.google.com", "80");
    if (sock == -1) {
        exit(EXIT_FAILURE);
    }
    std::string req = "GET /robots.txt HTTP/1.0\r\n\r\n";
    auto nbytes = ctx->send( //
          sock, req.c_str(), (remk::platform::Size)req.size(), 0);
    if (nbytes < 0 || (size_t)nbytes != req.size()) {
        REMK_PLATFORM_WARN(ctx, "send: retval=" << nbytes);
        (void)ctx->closesocket(sock);
        exit(EXIT_FAILURE);
    }
    char buffer[7];
    nbytes = ctx->recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (nbytes <= 0) {
        REMK_PLATFORM_WARN(ctx, "recv: retval=" << nbytes);
        (void)ctx->closesocket(sock);
        exit(EXIT_FAILURE);
    }
    buffer[(size_t)nbytes] = '\0';
    REMK_PLATFORM_INFO(ctx,
          "recv: nbytes=" << nbytes
                          << " data=" << ctx->hexdump(buffer, (size_t)nbytes));
    auto end = ctx->steady_clock_now();
    REMK_PLATFORM_INFO(ctx, "elapsed time: " << end - begin);
    (void)ctx->closesocket(sock);
    exit(EXIT_SUCCESS);
}

#include <remk/platform.hpp>

#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>

int main() {
    auto ctx = std::make_unique<remk::platform::Context>();
    if (ctx->wsainit() != 0) {
        REMK_PLATFORM_WARN(ctx, "wsainit");
        exit(EXIT_FAILURE);
    }
    int sock = ctx->connect_tcp("www.google.com", "80");
    if (sock == -1) {
        exit(EXIT_FAILURE);
    }
    remk::platform::DeferClosesocket dcs{ctx.get(), sock};
    std::string req = "GET /robots.txt HTTP/1.0\r\n\r\n";
    auto nbytes = ctx->writen(
          sock, req.c_str(), (remk::platform::Size)req.size(), 0);
    if (nbytes < 0 || (size_t)nbytes != req.size()) {
        REMK_PLATFORM_WARN(ctx, "send: retval=" << nbytes);
        exit(EXIT_FAILURE);
    }
    char buffer[7];
    nbytes = ctx->readn(sock, buffer, sizeof(buffer) - 1, 0);
    if (nbytes <= 0) {
        REMK_PLATFORM_WARN(ctx, "recv: retval=" << nbytes);
        exit(EXIT_FAILURE);
    }
    buffer[(size_t)nbytes] = '\0';
    REMK_PLATFORM_INFOX(ctx, "recv: nbytes=" << nbytes << " data=" << buffer);
    exit(EXIT_SUCCESS);
}

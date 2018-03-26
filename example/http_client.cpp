#include <remk/platform/platform.h>

#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>

int main() {
    auto ctx = std::make_unique<remk::platform::Context>();
    if (ctx->wsainit() != 0) {
        REMK_PLATFORM_WARN("wsainit");
        exit(EXIT_FAILURE);
    }
    addrinfo hints{};
    hints.ai_flags |= AI_NUMERICSERV;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *rp = nullptr;
    int rv = ctx->getaddrinfo("www.google.com", "80", &hints, &rp);
    if (rv != 0) {
        REMK_PLATFORM_WARNX("getaddrinfo: " << rv);
        exit(EXIT_FAILURE);
    }
    REMK_PLATFORM_INFOX("getaddrinfo: success");
    remk::platform::Socket sock = -1;
    for (auto ai = rp; ai != nullptr; ai = ai->ai_next) {
        sock = ctx->socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock == -1) {
            REMK_PLATFORM_WARN("socket");
            continue;
        }
        REMK_PLATFORM_INFOX("socket: success");
        if (ctx->connect(sock, ai->ai_addr,
                  (remk::platform::Socklen)ai->ai_addrlen) == 0) {
            REMK_PLATFORM_INFOX("connect: success");
            break;
        }
        REMK_PLATFORM_WARN("connect");
        ctx->closesocket(sock);
        sock = -1;
    }
    ctx->freeaddrinfo(rp);
    if (sock == -1) {
        REMK_PLATFORM_WARNX("all connect attempts failed");
        exit(EXIT_FAILURE);
    }
    std::string req = "GET /robots.txt HTTP/1.0\r\n\r\n";
    auto nbytes = ctx->send(
          sock, req.c_str(), (remk::platform::Size)req.size(), 0);
    if (nbytes < 0 || (size_t)nbytes != req.size()) {
        REMK_PLATFORM_WARN("send: retval=" << nbytes);
        ctx->closesocket(sock);
        exit(EXIT_FAILURE);
    }
    char buffer[7];
    nbytes = ctx->recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (nbytes <= 0) {
        REMK_PLATFORM_WARN("recv: retval=" << nbytes);
        ctx->closesocket(sock);
        exit(EXIT_FAILURE);
    }
    buffer[(size_t)nbytes] = '\0';
    REMK_PLATFORM_INFOX("recv: nbytes=" << nbytes << " data=" << buffer);
    ctx->closesocket(sock);
    exit(EXIT_SUCCESS);
}

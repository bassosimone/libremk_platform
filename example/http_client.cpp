#include <remk/platform/platform.h>

#include <stdlib.h>

#include <iostream>
#include <string>

int main() {
    if (remk_platform_wsainit() != 0) {
        REMK_PLATFORM_WARN("wsainit");
        exit(EXIT_FAILURE);
    }
    addrinfo hints{};
    hints.ai_flags |= AI_NUMERICSERV;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *rp = nullptr;
    int rv = remk_platform_getaddrinfo("www.google.com", "80", &hints, &rp);
    if (rv != 0) {
        REMK_PLATFORM_WARNX("getaddrinfo: " << rv);
        exit(EXIT_FAILURE);
    }
    REMK_PLATFORM_INFOX("getaddrinfo: success");
    remk_platform_socket_t sock = -1;
    for (auto ai = rp; ai != nullptr; ai = ai->ai_next) {
        sock = remk_platform_socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock == -1) {
            REMK_PLATFORM_WARN("socket");
            continue;
        }
        REMK_PLATFORM_INFOX("socket: success");
        if (remk_platform_connect(sock, ai->ai_addr,
                  (remk_platform_socklen_t)ai->ai_addrlen) == 0) {
            REMK_PLATFORM_INFOX("connect: success");
            break;
        }
        REMK_PLATFORM_WARN("connect");
        remk_platform_closesocket(sock);
        sock = -1;
    }
    remk_platform_freeaddrinfo(rp);
    if (sock == -1) {
        REMK_PLATFORM_WARNX("all connect attempts failed");
        exit(EXIT_FAILURE);
    }
    std::string req = "GET /robots.txt HTTP/1.0\r\n\r\n";
    auto nbytes = remk_platform_send(
          sock, req.c_str(), (remk_platform_size_t)req.size(), 0);
    if (nbytes < 0 || (size_t)nbytes != req.size()) {
        REMK_PLATFORM_WARN("send: retval=" << nbytes);
        remk_platform_closesocket(sock);
        exit(EXIT_FAILURE);
    }
    char buffer[7];
    nbytes = remk_platform_recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (nbytes <= 0) {
        REMK_PLATFORM_WARN("recv: retval=" << nbytes);
        remk_platform_closesocket(sock);
        exit(EXIT_FAILURE);
    }
    buffer[(size_t)nbytes] = '\0';
    REMK_PLATFORM_INFOX("recv: nbytes=" << nbytes << " data=" << buffer);
    remk_platform_closesocket(sock);
    exit(EXIT_SUCCESS);
}

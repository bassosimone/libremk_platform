#include <remk/platform/platform.hpp>

#include <stdlib.h>

#include <iostream>
#include <string>

int main() {
  addrinfo hints{};
  hints.ai_flags |= AI_NUMERICSERV;
  hints.ai_socktype = SOCK_STREAM;
  addrinfo *rp = nullptr;
  int rv = remk_platform_getaddrinfo("www.google.com", "80", &hints, &rp);
  if (rv != 0) {
    std::clog << "getaddrinfo (retval): " << rv << "\n";
    exit(EXIT_FAILURE);
  }
  std::clog << "getaddrinfo: success\n";
  remk_platform_socket_t sock = -1;
  for (auto ai = rp; ai != nullptr; ai = ai->ai_next) {
    sock = remk_platform_socket(ai->ai_family, ai->ai_socktype, 0);
    if (sock == -1) {
      std::clog << "socket (errno): " << remk_platform_get_last_error() << "\n";
      continue;
    }
    std::clog << "socket: success\n";
    if (remk_platform_connect(sock, ai->ai_addr,
        (remk_platform_socklen_t)ai->ai_addrlen) == 0) {
      std::clog << "connect: success\n";
      break;
    }
    std::clog << "connect (errno): " << remk_platform_get_last_error() << "\n";
    remk_platform_closesocket(sock);
    sock = -1;
  }
  remk_platform_freeaddrinfo(rp);
  if (sock == -1) {
    exit(EXIT_FAILURE);
  }
  std::string req = "GET /robots.txt HTTP/1.0\r\n\r\n";
  auto nbytes = remk_platform_send(
      sock, req.c_str(), (remk_platform_size_t)req.size(), 0);
  if (nbytes < 0 || (size_t)nbytes != req.size()) {
    std::clog << "send (retval): " << nbytes << "\n";
    std::clog << "send (errno): " << remk_platform_get_last_error() << "\n";
    remk_platform_closesocket(sock);
    exit(EXIT_FAILURE);
  }
  char buffer[7];
  nbytes = remk_platform_recv(sock, buffer, sizeof(buffer) - 1, 0);
  if (nbytes <= 0) {
    std::clog << "recv (retval): " << nbytes << "\n";
    std::clog << "recv (errno): " << remk_platform_get_last_error() << "\n";
    remk_platform_closesocket(sock);
    exit(EXIT_FAILURE);
  }
  buffer[(size_t)nbytes] = '\0';
  std::clog << "recv (nbytes): " << nbytes << "\n";
  std::clog << "recv (data): " << buffer << "\n";
  remk_platform_closesocket(sock);
  exit(EXIT_SUCCESS);
}

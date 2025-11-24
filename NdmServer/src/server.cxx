#ifndef NDM_SERVER_CXX
#define NDM_SERVER_CXX
#include "server.h"
#include "middlewares/middleware_base.h"
#include "middlewares/request_context.h"
#include "middlewares/response_context.h"
#include <algorithm>
#include <cstdio>
#include <exception>
#include <memory>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ndm::NdmServer::NdmServer() {}
ndm::NdmServer::~NdmServer() {
  _isRunning = false;
  for (auto sock : _listenTcpSockets) {
    if (shutdown(sock, SHUT_RDWR) != 0) {
    }
    if (close(sock) == -1) {
    }
  }
  for (auto sock : _listenUdpSockets) {
    if (shutdown(sock, SHUT_RDWR) != 0) {
    }
    if (close(sock) == -1) {
    }
  }
}

void ndm::NdmServer::addTcp(int port) {
  int tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (tcp_sock == -1) {
    throw std::runtime_error("create socket failure");
  }
  struct sockaddr_in tcp_addr;
  tcp_addr.sin_family = AF_INET;
  tcp_addr.sin_port = htons(port);
  tcp_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(tcp_sock, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) == -1) {
    throw std::runtime_error("bind socket failure. Try change target port");
  }
  _listenTcpSockets.push_back(tcp_sock);
}
void ndm::NdmServer::addUdp(int port) {}

void ndm::NdmServer::run() {
  _isRunning = true;
  for (int sock : _listenTcpSockets) {
    if (listen(sock, CONNECTION_MAX_SIZE) == -1) {
      throw std::runtime_error("listen");
    }
  }
  for (int sock : _listenUdpSockets) {
    if (listen(sock, CONNECTION_MAX_SIZE) == -1) {
      throw std::runtime_error("listen");
    }
  }

  struct epoll_event events[MAX_EVENTS];
  int conn_sock, nfds, epollfd;

  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    throw std::runtime_error("epoll create");
  }
  struct epoll_event ev{EPOLLIN};
  for (int sock : _listenUdpSockets) {
    ev.data.fd = sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev) == -1) {
      std::runtime_error("EPOLL ADD SOCKET");
    }
  }
  ev.events |= EPOLLHUP;
  for (int sock : _listenTcpSockets) {
    ev.data.fd = sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev) == -1) {
      std::runtime_error("EPOLL ADD SOCKET");
    }
  }

  while (_isRunning) {

    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (std::find(_listenTcpSockets.begin(), _listenTcpSockets.end(),
                    events[n].data.fd) != _listenTcpSockets.end()) {
        if (tcpConnect(events[n].data.fd, epollfd) == -1) {
          // Warning log
        }
        _users[events[n].data.fd] = User{};
      } else if (std::find(_listenUdpSockets.begin(), _listenUdpSockets.end(),
                           events[n].data.fd) != _listenUdpSockets.end()) {

        int user_sock = events[n].data.fd;
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        char buf[BUFFER_SIZE];
        ssize_t size_buf = recvfrom(user_sock, buf, BUFFER_SIZE - 1, 0,
                                    (sockaddr *)&addr, &addrlen);
        if (size_buf != -1) {
          if (_users.find(user_sock) == _users.end())
            _users[user_sock] = User{};
          else
            _users[user_sock].update_time();

          buf[size_buf] = '\0';

          requestHandle(buf, size_buf + 1,
                        [&user_sock, &addr, &addrlen](char *buf, int n) {
                          return sendto(user_sock, buf, n, 0, (sockaddr *)&addr,
                                        addrlen);
                        });
        }

      } else {
        int user_sock = events[n].data.fd;

        char buf[BUFFER_SIZE];

        ssize_t size_buf = recv(user_sock, buf, BUFFER_SIZE - 1, 0);
        // Warning if message length > sizebuf-1. Refactoring
        if (size_buf != -1) {
          if (size_buf == 0) {
            _users[user_sock].is_closed = true;
          } else {
            if (_users.find(user_sock) == _users.end())
              _users[user_sock] = User{};
            else
              _users[user_sock].update_time();

            buf[size_buf] = '\0';
            requestHandle(buf, size_buf + 1, [user_sock](char *buf, int n) {
              return send(user_sock, buf, n, 0);
            });
          }
        }
      }
    }
  }

  ev.events = EPOLLIN;
  for (int sock : _listenUdpSockets) {
    ev.data.fd = sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sock, &ev) == -1) {
      std::runtime_error("EPOLL DEL SOCKET");
    }
  }
  ev.events |= EPOLLHUP;
  for (int sock : _listenTcpSockets) {
    ev.data.fd = sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sock, &ev) == -1) {
      std::runtime_error("EPOLL DEL SOCKET");
    }
  }
  close(epollfd);
}
int ndm::NdmServer::tcpConnect(int sock, int epollfd) {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);

  auto conn_sock = accept(sock, (struct sockaddr *)&addr, &addrlen);
  if (conn_sock == -1) {
    return -1;
  }
  static struct epoll_event ev{EPOLLIN | EPOLLET};
  ev.data.fd = conn_sock;
  return epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev);
}
ssize_t
ndm::NdmServer::requestHandle(const char *buf, const int &size_buf,
                              std::function<ssize_t(char *, int)> send) {
  if (_root_middleware == nullptr) {
    return 0;
  }
  auto request = std::make_shared<RequestContext>(buf, size_buf);
  request->setUsers(&_users);

  auto response = std::make_shared<ResponseContext>();

  _root_middleware->handle_request(request, response);
  if (response->canShutdown()) {
    _isRunning = false;
  }
  if (send(response->response.data(), response->response.size()) == -1) {
    return -1;
  }

  return 0;
}

void ndm::NdmServer::setRootMiddleware(
    std::shared_ptr<ndm::MiddlewareBase> value) {
  _root_middleware = value;
}
std::shared_ptr<ndm::MiddlewareBase> ndm::NdmServer::getRootMiddleware() {
  return _root_middleware;
}

#endif // !NDM_SERVER_CXX

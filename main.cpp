#include <cerrno>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ostream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10
#define PORT 1010
#define BUFFER_SIZE 1024

void handle_error(std::string message) { std::cout << message << std::endl; }

int main(int argc, char *argv[]) {
  struct sockaddr_in tcp_addr;
  int tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (tcp_sock == -1) {
    handle_error("tcp_sock");
    return -1;
  }
  tcp_addr.sin_family = AF_INET;
  tcp_addr.sin_port = htons(PORT);
  tcp_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(tcp_sock, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) == -1) {
    handle_error("bind");
    return -1;
  }
  if (listen(tcp_sock, 5) == -1) {
    handle_error("listen");
    return -1;
  }

  struct epoll_event ev, events[MAX_EVENTS];
  int conn_sock, nfds, epollfd;

  /* Code to set up listening socket, 'listen_sock',
     (socket(), bind(), listen()) omitted. */

  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN;
  ev.data.fd = tcp_sock;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, tcp_sock, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == tcp_sock) {
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        conn_sock = accept(tcp_sock, (struct sockaddr *)&addr, &addrlen);
        if (conn_sock == -1) {
          perror("accept");
          exit(EXIT_FAILURE);
        }
        // setnonblocking(conn_sock);
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = conn_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
          perror("epoll_ctl: conn_sock");
          exit(EXIT_FAILURE);
        }
      } else {

        int client_sock = events[n].data.fd;
        char buf[BUFFER_SIZE];
        ssize_t size_buf = recv(client_sock, buf, BUFFER_SIZE - 1, 0);
        if (size_buf != -1) {
          buf[size_buf] = '\0';
          send(client_sock, buf, size_buf, 0);
        }
      }
    }
  }
  return 0;
}

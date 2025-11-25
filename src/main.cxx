#include "middlewares/command_middleware.h"
#include "middlewares/mirror_middleware.h"
#include "server.h"
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ostream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10
#define PORT 1011
#define BUFFER_SIZE 1024
std::string getCurrentDateTime() {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}
int main(int argc, char *argv[]) {
  ndm::NdmServer server{};
  server.addTcp(PORT);
  server.addUdp(PORT);
  auto mirror = std::make_shared<ndm::MirrorMiddleware>();
  auto rootMiddleware = std::make_shared<ndm::CommandMiddleware>();
  rootMiddleware->addCommand(
      "shutdown", [](std::shared_ptr<const ndm::RequestContext> request,
                     std::shared_ptr<ndm::ResponseContext> response) {
        response->shutdown();
      });
  rootMiddleware->addCommand(
      "stats", [](std::shared_ptr<const ndm::RequestContext> request,
                  std::shared_ptr<ndm::ResponseContext> response) {
        response->response = std::to_string(request->getConnectedUserCount()) +
                             '/' + std::to_string(request->getAllUserCount());
      });

  rootMiddleware->addCommand(
      "time", [](std::shared_ptr<const ndm::RequestContext> request,
                 std::shared_ptr<ndm::ResponseContext> response) {
        response->response = getCurrentDateTime();
      });

  rootMiddleware->setSuccessor(mirror);
  server.setRootMiddleware(rootMiddleware);
  server.run(10);

  return 0;
}

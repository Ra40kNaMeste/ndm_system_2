#include "middlewares/command_middleware.h"
#include "middlewares/mirror_middleware.h"
#include "server.h"
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
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

namespace po = boost::program_options;

#define MAX_EVENTS 10
#define PORT 1011

std::string getCurrentDateTime();
std::shared_ptr<ndm::MiddlewareBase> makeRootMiddleware();
int parseCommandArgments(int argc, char *argv[], po::variables_map &vm);

int main(int argc, char *argv[]) {
  po::variables_map vm;
  if (parseCommandArgments(argc, argv, vm)) {
    return 1;
  }
  int port = vm["port"].as<int>();
  int thread_count = vm["thread_count"].as<int>();

  ndm::NdmServer server{};
  server.addTcp(port);
  server.addUdp(port);
  server.setRootMiddleware(makeRootMiddleware());
  std::cout << "opened server on the port " << port << std::endl;
  server.run(thread_count);
  return 0;
}

int parseCommandArgments(int argc, char *argv[], po::variables_map &vm) {
  int port, buffer_size, thread_count;
  po::options_description desc("Allowed options");
  desc.add_options()("help", "Show this message")(
      "port,p", po::value<int>(&port)->default_value(1010), "port of server")(
      "thread_count,t", po::value<int>(&thread_count)->default_value(10),
      "received thread count");
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return -1;
  }
  return 0;
}

std::string getCurrentDateTime() {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}
std::shared_ptr<ndm::MiddlewareBase> makeRootMiddleware() {
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
  return rootMiddleware;
}

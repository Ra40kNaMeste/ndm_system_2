#ifndef NDM_SERVER_H
#define NDM_SERVER_H

#include "middlewares/middleware_base.h"
#include <ctime>
#include <functional>
#include <map>
#include <memory>
#include <pthread.h>
#include <sys/types.h>
#include <vector>
namespace ndm {

#define MAX_EVENTS 10
#define CONNECTION_MAX_SIZE 10
#define BUFFER_SIZE 1024
class NdmServer {
public:
  NdmServer();
  ~NdmServer();
  void addTcp(int port);
  void addUdp(int port);
  void run(int thread_count);
  void stop();

  void setRootMiddleware(std::shared_ptr<MiddlewareBase> value);
  std::shared_ptr<MiddlewareBase> getRootMiddleware();

private:
  void setIsRunning(bool value);
  pthread_mutex_t _isRunningMutex;
  bool _isRunning = false;

  std::shared_ptr<MiddlewareBase> _root_middleware = nullptr;

  static int tcpConnect(int sock, int epollfd);
  ssize_t requestHandle(const char *buf, const int &size_buf,
                        std::function<ssize_t(char *, int)> send);
  std::vector<int> _listenTcpSockets;
  std::vector<int> _listenUdpSockets;

  void startListenSockets();
  static void *listenThread(void *args);

  std::map<int, User> _users;
};

} // namespace ndm
#endif // !NDM_SERVER_H

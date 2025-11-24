#ifndef RESPONSECONTEXT_H
#define RESPONSECONTEXT_H

#include <string>
namespace ndm {
class ResponseContext {
public:
  ResponseContext();
  ~ResponseContext();

  void shutdown();
  bool canShutdown();

  std::string response{};

private:
  bool _is_shutdown = false;
};

} // namespace ndm
#endif // !RESPONSEREQUEST_H

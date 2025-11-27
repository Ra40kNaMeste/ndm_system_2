#ifndef RESPONSECONTEXT_H
#define RESPONSECONTEXT_H

#include <string>
namespace ndm {
class ResponseContext {
public:
  ResponseContext();
  ~ResponseContext();

  // Вызывать, когда необходимо прервать работу сервера
  void shutdown();

  bool canShutdown();

  // Ответ сервера записывать сюда
  std::string response{};

private:
  bool _is_shutdown = false;
};

} // namespace ndm
#endif // !RESPONSEREQUEST_H

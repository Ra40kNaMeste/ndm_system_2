#ifndef RESPONSECONTEXT_H
#define RESPONSECONTEXT_H

#include <string>
namespace ndm {
class ResponseContext {
public:
  ResponseContext();
  ~ResponseContext();

  // Вызывается, когда надо закрыть приложение
  void shutdown();

  bool canShutdown();

  // Ответ сервера
  std::string response{};

private:
  bool _is_shutdown = false;
};

} // namespace ndm
#endif // !RESPONSEREQUEST_H

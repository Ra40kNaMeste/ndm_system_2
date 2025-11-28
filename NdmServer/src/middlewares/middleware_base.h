#ifndef IMIDDLEWARE_H
#define IMIDDLEWARE_H

#include "request_context.h"
#include "response_context.h"
#include <memory>
namespace ndm {
class MiddlewareBase {
public:
  MiddlewareBase() {}
  virtual ~MiddlewareBase() {}

  // Обработка запроса
  virtual void handle_request(std::shared_ptr<const RequestContext> request,
                              std::shared_ptr<ResponseContext> response) {}

  std::shared_ptr<MiddlewareBase> getSuccessor() { return _successor; }
  void setSuccessor(std::shared_ptr<MiddlewareBase> value) {
    _successor = value;
  }

protected:
  std::shared_ptr<MiddlewareBase> _successor = nullptr;
};

} // namespace ndm
#endif // !IMIDDLEWARE_H

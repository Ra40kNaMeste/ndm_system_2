#ifndef MIRRORMIDDLEWARE_H
#define MIRRORMIDDLEWARE_H

#include "middleware_base.h"
namespace ndm {
class MirrorMiddleware : public MiddlewareBase {
public:
  MirrorMiddleware() : MiddlewareBase() {}
  ~MirrorMiddleware() {}

  void handle_request(std::shared_ptr<const RequestContext> request,
                      std::shared_ptr<ResponseContext> response) override;
};

} // namespace ndm

#endif // !MIRRORMIDDLEWARE_H

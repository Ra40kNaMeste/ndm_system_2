#ifndef MIRRORMIDDLEWARE_CXX
#define MIRRORMIDDLEWARE_CXX
#include "mirror_middleware.h"
#include <iostream>
void ndm::MirrorMiddleware::handle_request(
    std::shared_ptr<const RequestContext> request,
    std::shared_ptr<ResponseContext> response) {
  response->response = request->getMessage();
}

#endif // !MIRRORMIDDLEWARE_CXX

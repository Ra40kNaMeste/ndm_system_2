#ifndef RESPONSECONTEXT_CXX
#define RESPONSECONTEXT_CXX
#include "response_context.h"
ndm::ResponseContext::ResponseContext() {}
ndm::ResponseContext::~ResponseContext() {}
void ndm::ResponseContext::shutdown() { _is_shutdown = true; }
bool ndm::ResponseContext::canShutdown() { return _is_shutdown; }
#endif // !RESPONSECONTEXT_CXX

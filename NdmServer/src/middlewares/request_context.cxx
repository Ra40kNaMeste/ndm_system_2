#ifndef REQUESTCONTEXT_CXX
#define REQUESTCONTEXT_CXX
#include "request_context.h"

ndm::RequestContext::RequestContext(const char *buf, const int size_buf,
                                    const std::map<int, User> *users)
    : _size_buf(size_buf) {
  _users = users;
  _buf = buf;
}
ndm::RequestContext::~RequestContext() {}

unsigned int ndm::RequestContext::getConnectedUserCount() const {
  return std::count_if(
      _users->begin(), _users->end(),
      [](const std::pair<int, User> i) { return !i.second.canClosed(); });
}

unsigned int ndm::RequestContext::getAllUserCount() const {
  return _users->size();
}

void ndm::RequestContext::getData(const char *buf, int &size_buf) {
  buf = _buf;
  size_buf = _size_buf;
}
std::string ndm::RequestContext::getMessage() const {
  if (_buf[_size_buf - 1] == '\0')
    return std::string{_buf};
  else {
    return std::string(_buf, _size_buf);
  }
}

#endif // !REQUESTCONTEXT_CXX

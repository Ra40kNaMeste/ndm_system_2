#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H
#include "user.h"
#include <algorithm>
#include <ctime>
#include <map>
#include <string>
namespace ndm {

class RequestContext {
public:
  RequestContext(const char *buf, const int size_buf);
  ~RequestContext();

  void setUsers(const std::map<int, User> *users);

  unsigned int getConnectedUserCount() const;

  unsigned int getAllUserCount() const;

  void getData(const char *buf, int &size_buf);
  std::string getMessage() const;

  const std::map<int, User> *_users;

private:
  const char *_buf;
  const int _size_buf;
};

} // namespace ndm

#endif // !REQUESTCONTEXT_H

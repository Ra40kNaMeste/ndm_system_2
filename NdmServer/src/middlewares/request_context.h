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
  RequestContext(const char *buf, const int size_buf,
                 const std::map<int, User> *users);
  ~RequestContext();

  // Возвращает количество подключённых пользователей
  unsigned int getConnectedUserCount() const;

  // Возвращает количество всех пользователей
  unsigned int getAllUserCount() const;

  // Возвращает сообщение в виде буфера
  void getData(const char *buf, int &size_buf);

  // Возвращает сообщение в виде строки
  std::string getMessage() const;

  const std::map<int, User> *_users;

private:
  const char *_buf;
  const int _size_buf;
};

} // namespace ndm

#endif // !REQUESTCONTEXT_H

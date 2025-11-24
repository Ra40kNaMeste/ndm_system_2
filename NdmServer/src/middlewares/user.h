#ifndef USER_H
#define USER_H
#include <chrono>
#include <ctime>
namespace ndm {

#define MAX_CONNECTION_TIMEOUT 5
class User {
public:
  User() { update_time(); }

  bool canClosed() const;
  void update_time();

  std::chrono::steady_clock::time_point last_time;
  bool is_closed = false;
};

} // namespace ndm
#endif // !USER_H

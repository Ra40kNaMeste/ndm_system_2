#ifndef USER_H
#define USER_H
#include <chrono>
#include <ctime>
namespace ndm {

// Таймаут, в течении которого клиент считается подключённым
#define MAX_CONNECTION_TIMEOUT 5
class User {
public:
  User() { update_time(); }

  // возвращает, отключён ли клиент
  bool canClosed() const;
  // Вызывать при поключении клиента
  void update_time();

  std::chrono::steady_clock::time_point last_time;
  // изменять на true только когда будет понятно, что клиент отключился
  bool is_closed = false;
};

} // namespace ndm
#endif // !USER_H

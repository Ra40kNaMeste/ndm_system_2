#ifndef USER_CXX
#define USER_CXX
#include "user.h"
#include <chrono>
bool ndm::User::canClosed() const {
  auto t = std::chrono::steady_clock::now();
  auto dur = t - last_time;
  return is_closed || std::chrono::duration<double>(t - last_time).count() >
                          MAX_CONNECTION_TIMEOUT;
}
void ndm::User::update_time() {
  is_closed = false;
  last_time = std::chrono::steady_clock::now();
}

#endif // !USER_CXX

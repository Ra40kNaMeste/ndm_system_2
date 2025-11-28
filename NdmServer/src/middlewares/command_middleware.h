#ifndef COMMANDMIDDLEWARE_H
#define COMMANDMIDDLEWARE_H

#include "middleware_base.h"
#include "request_context.h"
#include "response_context.h"
#include <functional>
#include <map>
#include <memory>
#include <set>

namespace ndm {
// Выполняет команды. Команды регистрируются в addCommand. Все команды
// начинаются на /. Если команда не найдена - сообщает об этом. Если сообщение -
// не команда - передаёт дальше
class CommandMiddleware : public MiddlewareBase {
public:
  CommandMiddleware() : MiddlewareBase() {}
  ~CommandMiddleware() {}

  void handle_request(std::shared_ptr<const RequestContext> request,
                      std::shared_ptr<ResponseContext> response) override;

  void addCommand(std::string name,
                  std::function<void(std::shared_ptr<const RequestContext>,
                                     std::shared_ptr<ResponseContext>)>
                      command);
  void removeCommand(std::string name);

private:
  std::map<std::string,
           std::function<void(std::shared_ptr<const RequestContext>,
                              std::shared_ptr<ResponseContext>)>>
      _commands;
};

} // namespace ndm

#endif // !COMMANDMIDDLEWARE_H

#ifndef COMMANDMIDDLEWARE_CXX
#define COMMANDMIDDLEWARE_CXX

#include "command_middleware.h"
#include "request_context.h"
#include "response_context.h"
#include <iostream>
#include <string>
void ndm::CommandMiddleware::handle_request(
    std::shared_ptr<const RequestContext> request,
    std::shared_ptr<ResponseContext> response) {
  std::string message = request->getMessage();
  if (message.size() != 0 && message[0] == '/') {
    std::cout << "That's command!\n";
    std::string command = message.substr(1);
    command.erase(std::find_if(command.rbegin(), command.rend(),
                               [](unsigned char ch) {
                                 return !std::isspace(ch) && ch != '\n' &&
                                        ch != '\r';
                               })
                      .base(),
                  command.end());
    std::cout << "find command=" << command << "|\n";
    auto index = _commands.find(command);
    if (index != _commands.end()) {
      std::cout << "command is finded\n";
      index->second(request, response);
      return;
    }
  }
  if (_successor != nullptr)
    _successor->handle_request(request, response);
}

void ndm::CommandMiddleware::addCommand(
    std::string name,
    std::function<void(std::shared_ptr<const ndm::RequestContext>,
                       std::shared_ptr<ndm::ResponseContext>)>
        command) {
  _commands[name] = command;
}
void ndm::CommandMiddleware::removeCommand(std::string name) {
  _commands.erase(name);
}

#endif // !COMMANDMIDDLEWARE_CXX

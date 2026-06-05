#pragma once

#include "ExceptionHandler.h"
#include "ICommand.h"

#include <queue>
#include <vector>

class EventLoop {
 public:
  void push(ICommandPtr cmd) {
    _queue.push(std::move(cmd));
  }

  void handle_commands() {
    while (!_queue.empty()) {
      auto cmd = _queue.front();
      _queue.pop();
      try {
        //1. Обернуть вызов Команды в блок try-catch.
        cmd->execute();
      } catch (const std::exception &e) { // 2.Обработчик catch должен перехватывать только самое базовое исключение.
        // 3. Есть множество различных обработчиков исключений. Выбор подходящего
        // обработчика исключения делается на основе экземпляра перехваченного исключения и команды, которая выбросила исключение.
        auto handler = g_exceptionHandler.handle(cmd, e);
        if (handler) {
          for (auto &nc : handler->execute()) {
            _queue.push(std::move(nc));
          }
        }
      }
    }
  }

 private:
  std::queue<ICommandPtr> _queue;
};

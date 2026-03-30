#pragma once

#include "ExceptionHandler.h"
#include "ICommand.h"

#include <queue>

class Executor {
 public:
  void handle_commands() {
    while (_active && !_queue.empty()) {
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
          handler->execute();
        }
      }
    }
  }

  std::queue<ICommandPtr> _queue;

 private:
  bool _active = true;
};

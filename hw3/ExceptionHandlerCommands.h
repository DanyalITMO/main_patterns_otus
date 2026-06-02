#pragma once

#include "Commands.h"
#include "ExceptionHandler.h"

#include <queue>
#include <string>

//5. Реализовать обработчик исключения, который ставит Команду, пишущую в лог в очередь Команд.
class LogHandlerCommand : public IExceptionHandlerCommand {
 public:
  explicit LogHandlerCommand(std::queue<ICommandPtr> &queue,
                             const std::exception &ex)
      : _queue{queue}, _message{ex.what()} {}

  void execute() noexcept final {
    _queue.push(std::make_shared<LogCommand>(_message));
  }

 private:
  std::queue<ICommandPtr> &_queue;
  std::string _message;
};

//7. Реализовать обработчик исключения, который ставит в очередь Команду - повторитель команды, выбросившей исключение.
class RepeatHandlerCommand : public IExceptionHandlerCommand {
public:
  RepeatHandlerCommand(std::queue<ICommandPtr> &queue,
                              ICommandPtr failed)
      : _queue{queue}, _failed{std::move(failed)} {}

  void execute() noexcept final {
    _queue.push(std::make_shared<RepeatCommand>(_failed));
  }

private:
  std::queue<ICommandPtr> &_queue;
  ICommandPtr _failed;
};

class RepeatTwiceHandlerCommand : public IExceptionHandlerCommand {
public:
  RepeatTwiceHandlerCommand(std::queue<ICommandPtr> &queue,
                              ICommandPtr failed)
      : _queue{queue}, _failed{std::move(failed)} {}

  void execute() noexcept final {
    _queue.push(std::make_shared<RepeatTwiceCommand>(_failed));
  }

private:
  std::queue<ICommandPtr> &_queue;
  ICommandPtr _failed;
};

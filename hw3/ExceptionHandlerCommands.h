#pragma once

#include "Commands.h"
#include "ExceptionHandler.h"

#include <string>
#include <vector>

//5. Реализовать обработчик исключения, который возвращает Команду, пишущую в лог.
class LogHandlerCommand : public IExceptionHandlerCommand {
 public:
  explicit LogHandlerCommand(const std::exception &ex)
      : _message{ex.what()} {}

  std::vector<ICommandPtr> execute() noexcept final {
    return {std::make_shared<LogCommand>(_message)};
  }

 private:
  std::string _message;
};

//7. Реализовать обработчик исключения, который возвращает Команду - повторитель команды, выбросившей исключение.
class RepeatHandlerCommand : public IExceptionHandlerCommand {
public:
  explicit RepeatHandlerCommand(ICommandPtr failed)
      : _failed{std::move(failed)} {}

  std::vector<ICommandPtr> execute() noexcept final {
    return {std::make_shared<RepeatCommand>(_failed)};
  }

private:
  ICommandPtr _failed;
};

class RepeatTwiceHandlerCommand : public IExceptionHandlerCommand {
public:
  explicit RepeatTwiceHandlerCommand(ICommandPtr failed)
      : _failed{std::move(failed)} {}

  std::vector<ICommandPtr> execute() noexcept final {
    return {std::make_shared<RepeatTwiceCommand>(_failed)};
  }

private:
  ICommandPtr _failed;
};

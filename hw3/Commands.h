#pragma once

#include "ICommand.h"
#include <iostream>

//4. Реализовать Команду, которая записывает информацию о выброшенном исключении в лог.
class LogCommand : public ICommand {
  public:
  LogCommand(const std::string& msg) : _msg(msg){}

  void execute() final {
    std::cerr<<"Log command"<<std::endl;
    std::cerr << _msg << std::endl;
  }

  std::string _msg;
};

//6. Реализовать Команду, которая повторяет Команду, выбросившую исключение.
class RepeatCommand : public ICommand {
public:
  RepeatCommand(ICommandPtr command) : _command(command) {}

  void execute() override {
    std::cerr<<"Repeat command execute"<<std::endl;
    _command->execute();
  }


  ICommandPtr _command;
};

class RepeatTwiceCommand : public ICommand {
public:
  explicit RepeatTwiceCommand(ICommandPtr command) : _command(std::move(command)) {}

  void execute() override {
    std::cerr<<"Repeat twice command::execute"<<std::endl;
    _command->execute();
  }

  ICommandPtr _command;
};
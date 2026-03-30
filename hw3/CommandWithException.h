#pragma once
#include "ICommand.h"

#include <iostream>
#include <stdexcept>


class CommandWithException : public ICommand {
  public:
  void execute() {
    throw std::runtime_error("CommandWithException");
  }
};
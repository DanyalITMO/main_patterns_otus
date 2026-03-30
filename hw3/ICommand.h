#pragma once
#include <memory>

class ICommand {
public:
  virtual ~ICommand() = default;
  virtual void execute()  = 0;
};

using ICommandPtr = std::shared_ptr<ICommand>;
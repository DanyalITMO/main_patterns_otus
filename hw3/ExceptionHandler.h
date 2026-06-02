#pragma once

#include "ICommand.h"

#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

struct TypePairHash {
  std::size_t operator()(
      const std::pair<std::type_index, std::type_index> &p) const noexcept {
    const std::size_t h1 = std::hash<std::type_index>{}(p.first);
    const std::size_t h2 = std::hash<std::type_index>{}(p.second);
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
  }
};

class IExceptionHandlerCommand {
public:
  virtual ~IExceptionHandlerCommand() = default;
  virtual void execute() noexcept = 0;
};


using IExceptionHandlerCommandPtr = std::shared_ptr<IExceptionHandlerCommand>;

class ExceptionHandler {
public:
  using ClassType = std::type_index;
  using ExceptionType = std::type_index;
  using HandlerKey = std::pair<ClassType, ExceptionType>;
  using func = std::function<IExceptionHandlerCommandPtr(const ICommandPtr &cmd, const std::exception &ex)>;

  void register_handle(const ICommandPtr &cmd, const std::exception &ex,
                       const func &handler) {
    _handlers.emplace(HandlerKey{typeid(*cmd), typeid(ex)}, handler);
  }

  void register_handle(const ICommand &cmd, const std::exception &ex,const func& handler) {
    _handlers.emplace(HandlerKey{typeid(cmd), typeid(ex)}, handler);
  }

  void unregister_handle(const ICommandPtr &cmd, std::exception &ex) {
    _handlers.erase(HandlerKey{typeid(*cmd), typeid(ex)});
  }

  void unregister_handle(const ICommand &cmd, std::exception &ex) {
    _handlers.erase(HandlerKey{typeid(cmd), typeid(ex)});
  }

  IExceptionHandlerCommandPtr handle(const ICommandPtr &cmd,
                                     const std::exception &ex) {
    return _handlers.at(HandlerKey{typeid(*cmd), typeid(ex)})(cmd, ex);
  }

  void clear() noexcept { _handlers.clear(); }

 private:
  std::unordered_map<HandlerKey, func, TypePairHash> _handlers;
};

static inline ExceptionHandler g_exceptionHandler;
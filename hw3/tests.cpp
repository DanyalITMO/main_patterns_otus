#include "Commands.h"
#include "EventLoop.h"
#include "ExceptionHandler.h"
#include "ExceptionHandlerCommands.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

namespace {

class ExceptionHandlerFixture : public ::testing::Test {
 protected:
  void SetUp() override { g_exceptionHandler.clear(); }
};

// --- Команда, пишущая в лог (сообщение об исключении) ---
TEST_F(ExceptionHandlerFixture, LogCommand_writes_exception_message_to_stderr) {
  testing::internal::CaptureStderr();
  LogCommand log("boom");
  log.execute();
  const std::string out = testing::internal::GetCapturedStderr();
  EXPECT_NE(out.find("Log command"), std::string::npos);
  EXPECT_NE(out.find("boom"), std::string::npos);
}

// --- Обработчик: возвращает команду лога ---
TEST_F(ExceptionHandlerFixture, LogHandlerCommand_execute_returns_LogCommand) {
  try {
    throw std::runtime_error("fire");
  } catch (const std::exception &e) {
    LogHandlerCommand handler(e);
    auto cmds = handler.execute();
    ASSERT_EQ(cmds.size(), 1u);
    testing::internal::CaptureStderr();
    cmds.front()->execute();
    const std::string out = testing::internal::GetCapturedStderr();
    EXPECT_NE(out.find("fire"), std::string::npos);
  }
}

// --- Команда-повторитель ---
class CountingCommand : public ICommand {
 public:
  explicit CountingCommand(int *counter) : counter_(counter) {}
  void execute() override { ++*counter_; }

 private:
  int *counter_;
};

TEST_F(ExceptionHandlerFixture, RepeatCommand_execute_delegates_to_inner_command) {
  int n = 0;
  auto inner = std::make_shared<CountingCommand>(&n);
  RepeatCommand repeater(inner);
  repeater.execute();
  EXPECT_EQ(n, 1);
  repeater.execute();
  EXPECT_EQ(n, 2);
}

// --- Обработчик: возвращает повторитель упавшей команды ---
class AlwaysThrowCommand : public ICommand {
 public:
  void execute() override { throw std::runtime_error("again"); }
};

TEST_F(ExceptionHandlerFixture, RepeatHandlerCommand_execute_returns_RepeatCommand_for_failed) {
  auto failed = std::make_shared<AlwaysThrowCommand>();
  RepeatHandlerCommand handler(failed);
  auto cmds = handler.execute();
  ASSERT_EQ(cmds.size(), 1u);
  auto repeat = cmds.front();
  ASSERT_NE(repeat, nullptr);
  EXPECT_THROW(repeat->execute(), std::runtime_error);
}

// --- Сквозной сценарий: после исключения в очереди оказывается лог ---
class ThrowRuntimeCommand : public ICommand {
 public:
  void execute() override { throw std::runtime_error("from_cmd"); }
};

TEST_F(ExceptionHandlerFixture, EventLoop_log_handler_runs_enqueued_log_command) {
  EventLoop eventLoop;
  g_exceptionHandler.register_handle(
      ThrowRuntimeCommand{}, std::runtime_error(""),
      [](const ICommandPtr &, const std::exception &ex) {
        return std::make_shared<LogHandlerCommand>(ex);
      });
  eventLoop.push(std::make_shared<ThrowRuntimeCommand>());
  testing::internal::CaptureStderr();
  eventLoop.handle_commands();
  const std::string out = testing::internal::GetCapturedStderr();
  EXPECT_NE(out.find("from_cmd"), std::string::npos);
}

// --- Сквозной сценарий: после исключения в очередь кладётся повторитель ---
struct CountThenThrow : ICommand {
  int& attempts;
  explicit CountThenThrow(int& a) : attempts(a) {}
  void execute() override {
    ++attempts;
    throw std::runtime_error("retry_then_log");
  }
};


//8. С помощью Команд из пункта 4 и пункта 6 реализовать следующую обработку исключений:
// при первом выбросе исключения повторить команду, при повторном выбросе исключения записать информацию в лог.
TEST_F(ExceptionHandlerFixture, EventLoop_retry_once_then_logs) {
  EventLoop eventLoop;
  static int typeid_stub = 0;
  g_exceptionHandler.register_handle(
      CountThenThrow{typeid_stub}, std::runtime_error(""),
      [](const ICommandPtr &cmd, const std::exception &) {
        return std::make_shared<RepeatHandlerCommand>(cmd);
      });
  g_exceptionHandler.register_handle(
      RepeatCommand{ICommandPtr{}}, std::runtime_error(""),
      [](const ICommandPtr &, const std::exception &ex) {
        return std::make_shared<LogHandlerCommand>(ex);
      });

  int attempts = 0;
  eventLoop.push(std::make_shared<CountThenThrow>(attempts));
  testing::internal::CaptureStderr();
  eventLoop.handle_commands();
  const std::string out = testing::internal::GetCapturedStderr();

  EXPECT_EQ(attempts, 2) << "исходная команда + один повтор через RepeatCommand";
  EXPECT_NE(out.find("retry_then_log"), std::string::npos)
      << "после второго падения должна выполниться LogCommand";
}


//9. Реализовать стратегию обработки исключения - повторить два раза, потом записать в лог.
//Указание: создать новую команду, точно такую же как в пункте 6. Тип этой команды будет показывать, что Команду не удалось выполнить два раза.
TEST_F(ExceptionHandlerFixture, EventLoop_retry_twice_then_logs) {
  EventLoop eventLoop;
  static int typeid_stub = 0;
  g_exceptionHandler.register_handle(
      CountThenThrow{typeid_stub}, std::runtime_error(""),
      [](const ICommandPtr &cmd, const std::exception &) {
        return std::make_shared<RepeatHandlerCommand>(cmd);
      });
  g_exceptionHandler.register_handle(
      RepeatCommand{ICommandPtr{}}, std::runtime_error(""),
      [](const ICommandPtr &cmd, const std::exception &) {
        return std::make_shared<RepeatTwiceHandlerCommand>(cmd);
      });
  g_exceptionHandler.register_handle(
      RepeatTwiceCommand{ICommandPtr{}}, std::runtime_error(""),
      [](const ICommandPtr &, const std::exception &ex) {
        return std::make_shared<LogHandlerCommand>(ex);
      });
  int attempts = 0;
  eventLoop.push(std::make_shared<CountThenThrow>(attempts));
  testing::internal::CaptureStderr();
  eventLoop.handle_commands();
  const std::string out = testing::internal::GetCapturedStderr();

  EXPECT_EQ(attempts, 3) << "исходная команда + 2 повтора через";
  EXPECT_NE(out.find("retry_then_log"), std::string::npos)
      << "после третьего падения должна выполниться LogCommand";
}


}  // namespace

#pragma once
#include "CommandException.h"
#include "ICommand.h"

#include <exception>
#include <vector>

class Macrocommand : public ICommand
{
public:

    Macrocommand(std::vector<ICommandPtr> cmds) : _cmds{std::move(cmds)} {}
    void execute() override {
        try {
            for (const auto& cmd : _cmds) {
                cmd->execute();
            }
        } catch (const CommandException&) {
            throw;
        } catch (const std::exception& e) {
            throw CommandException(e.what());
        } catch (...) {
            throw CommandException("Unknown command exception");
        }
    }
private:

    std::vector<ICommandPtr> _cmds;
};
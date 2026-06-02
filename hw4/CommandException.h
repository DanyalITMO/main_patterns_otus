#pragma once
#include <exception>
#include <string>

class CommandException : public std::exception
{
public:
    explicit CommandException(std::string msg)
        : _msg{std::move(msg)} {}

    const char* what() const noexcept override {
        return _msg.c_str();
    }

private:
    std::string _msg;
};
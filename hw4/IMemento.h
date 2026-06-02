#pragma once
#include <memory>

class IMemento
{
public:
    class Memento
    {
        public:
        virtual ~Memento()  = default;
    };

    virtual ~IMemento()  = default;
    public:
    virtual std::unique_ptr<Memento> create_memento() = 0;
    virtual void restore_from_memento(const std::unique_ptr<Memento>&) = 0;
};
#pragma once
#include <memory>

struct Vector
{
    int x;
    int y;
};

inline bool operator==(const Vector& lhs, const Vector& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

class IMovable
{
public:

    virtual ~IMovable() {};
    virtual Vector getLocation() = 0;
    virtual void setLocation(const Vector&) = 0;
    virtual Vector getVelocity() = 0;
};

using IMovablePtr = std::shared_ptr<IMovable>;

Vector operator+(const Vector& lhs, const Vector& rhs) { return Vector{lhs.x + rhs.x, lhs.y + rhs.y}; }
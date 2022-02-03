#ifndef VECTOR_2D_CPP
#define VECTOR_2D_CPP
#include "vector2d.h"

#include <math.h>

Vector2d::Vector2d(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2d operator+(const Vector2d &v, const Vector2d &other)
{
    return Vector2d(v.x + other.x, v.y + other.y);
}
Vector2d operator-(Vector2d v, Vector2d other)
{
    return v -= other;
    //return Vector2d(v.x - other.x, v.y - other.y);
}
Vector2d operator*(Vector2d v, float k)
{
    return Vector2d(v.x * k, v.y * k);
}
Vector2d operator/(Vector2d v, float k)
{
    return Vector2d(v.x / k, v.y / k);
}

float Vector2d::length() const
{
    return sqrt(x * x + y * y);
}
float Vector2d::length2() const
{
    return x * x + y * y;
}

Vector2d Vector2d::rotate(float angle) const
{
    float c = cos(angle);
    float s = sin(angle);
    float tx = x * c - y * s;
    float ty = x * s + y * c;

    return Vector2d(tx, ty);
}

Vector2d Vector2d::normalise() const
{
    return *this / length();
}

Vector2d Vector2d::perpendiculate() const
{
    return Vector2d(-y, x);
}

Vector2d vector_add(Vector2d a, Vector2d b)
#if 0
{
    __asm__(
        "movq   %xmm0,-0x28(%rsp)"
        "movq   %xmm1,-0x20(%rsp)"
        "movq   %xmm2,-0x18(%rsp)"
        "movq   %xmm3,-0x10(%rsp)"
        "movapd -0x18(%rsp),%xmm4"
        "addpd  -0x28(%rsp),%xmm4"
        "movaps %xmm4,-0x28(%rsp)"
        "movsd  -0x20(%rsp),%xmm1"
        "movsd  -0x28(%rsp),%xmm0");
}
#else
{
    a.x += b.x;
    a.y += b.y;
    return a;
}
#endif

Vector2d vector_sub(Vector2d a, Vector2d b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

Vector2d vector_mul(Vector2d v, float k)
{
    v.x *= k;
    v.y *= k;
    return v;
}

Vector2d vector_div(Vector2d v, float k)
{
    v.x /= k;
    v.y /= k;
    return v;
}

float vector_length(Vector2d v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}
float vector_length2(Vector2d v)
{
    return v.x * v.x + v.y * v.y;
}

Vector2d vector_rotate(Vector2d v, float angle)
{
    float x = v.x,
           y = v.y;
    float c = cos(angle);
    float s = sin(angle);
    v.x =  x * c - y * s;
    v.y =  x * s + y * c;

    return v;
}

#endif
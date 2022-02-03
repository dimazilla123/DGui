#ifndef VECTOR_2D_H
#define VECTOR_2D_H

class Vector2d
{
public:
    Vector2d(float x, float y);

    friend Vector2d operator+(const Vector2d &v, const Vector2d &other);
    friend Vector2d operator-(Vector2d v, Vector2d other);
    friend Vector2d operator*(Vector2d v, float k);
    friend Vector2d operator/(Vector2d v, float k);

    Vector2d &operator-=(const Vector2d &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    float length() const;
    float length2() const;

    Vector2d rotate(float angle) const;
    Vector2d normalise() const;

    Vector2d perpendiculate() const;

    float x, y;
private:
};

// Cmp with c-style

Vector2d vector_add(Vector2d a, Vector2d b);

Vector2d vector_sub(Vector2d a, Vector2d b);

Vector2d vector_mul(Vector2d v, float k);

Vector2d vector_div(Vector2d v, float k);

float vector_length(Vector2d v);
float vector_length2(Vector2d v);

Vector2d vector_rotate(Vector2d v, float angle);

#endif
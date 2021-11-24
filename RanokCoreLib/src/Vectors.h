#ifndef VECTORS_H
#define VECTORS_H

template<class T>
class Vector2
{
public:
    Vector2():
        Vector2(0, 0)
    {}
    Vector2(T x, T y):
        x(x), y(y)
    {}
    Vector2(const Vector2& rhs):
        x(rhs.x), y(rhs.y)
    {}


    T x;
    T y;


    Vector2& operator=(const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }
    Vector2 operator+(const Vector2& rhs)
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }
    Vector2 operator-(const Vector2& rhs)
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }
    Vector2 operator*(const Vector2& rhs)
    {
        return Vector2(x * rhs.x, y * rhs.y);
    }
    Vector2 operator*(const float& rhs)
    {
        return Vector2(x * rhs, y * rhs);
    }
    Vector2 operator/(const Vector2& rhs)
    {
        return Vector2(x / rhs.x, y / rhs.y);
    }
    Vector2 operator/(const float& rhs)
    {
        return Vector2(x / rhs, y / rhs);
    }

    Vector2& operator+=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Vector2& operator-=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Vector2 operator*=(const Vector2& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    Vector2 operator*=(const float& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    Vector2 operator/=(const Vector2& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }
    Vector2 operator/=(const float& rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }
};


template<class T>
class Vector3
{
public:
    Vector3():
        Vector3(0, 0, 0)
    {}
    Vector3(T x, T y, T z):
        x(x), y(y), z(z)
    {}
    Vector3(const Vector3& rhs):
        x(rhs.x), y(rhs.y), z(rhs.z)
    {}


    T x;
    T y;
    T z;


    Vector3& operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector3 operator+(const Vector3& rhs)
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Vector3 operator-(const Vector3& rhs)
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Vector3 operator*(const Vector3& rhs)
    {
        return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
    }
    Vector3 operator/(const Vector3& rhs)
    {
        return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
    }
    Vector3 operator*(const float& rhs)
    {
        return Vector3(x * rhs, y * rhs, z * rhs);
    }
    Vector3 operator/(const float& rhs)
    {
        return Vector3(x / rhs, y / rhs, z / rhs);
    }

    Vector3& operator+=(const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Vector3& operator-=(const Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Vector3 operator*=(const Vector3& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
    Vector3 operator/=(const Vector3& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }
    Vector3 operator*=(const float& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    Vector3 operator/=(const float& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }
};


using Vector2u = Vector2<unsigned>;
using Vector3u = Vector3<unsigned>;
using Vector2d = Vector2<double>;
using Vector3d = Vector3<double>;
using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector2i = Vector2<int>;
using Vector3i = Vector3<int>;


#endif // VECTORS_H

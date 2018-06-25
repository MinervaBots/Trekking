#ifndef VECTOR_H
#define VECTOR_H

#include <Arduino.h>

struct Vector3
{
    float X, Y, Z;

    Vector3()
    {
        set(0, 0, 0);
    }

    Vector3(float x, float y, float z)
    {
        set(x, y, z);
    }

    void set(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    float module(bool useX, bool useY, bool useZ)
    {
        float result = 0;

        if (useX)
        {
            result += pow(X, 2);
        }
        if (useY)
        {
            result += pow(Y, 2);
        }
        if (useZ)
        {
            result += pow(Z, 2);
        }

        return sqrt(result);
    }

    static Vector3 lerp(Vector3 vectorA, Vector3 vectorB, float alpha)
    {
        Vector3 resultVector;
        resultVector.X = ((1 - alpha) * vectorA.X + alpha * vectorB.X);
        resultVector.Y = ((1 - alpha) * vectorA.Y + alpha * vectorB.Y);
        resultVector.Z = ((1 - alpha) * vectorA.Z + alpha * vectorB.Z);
        return resultVector;
    }

    Vector3 operator+(const Vector3 &b)
    {
        Vector3 v;
        v.X = this->X + b.X;
        v.Y = this->Y + b.Y;
        v.Z = this->Z + b.Z;
        return v;
    }
    Vector3 &operator+=(const Vector3 &rhs)
    {
        X += rhs.X;
        Y += rhs.Y;
        Z += rhs.Z;
        return *this;
    }

    Vector3 &operator-=(const Vector3 &rhs)
    {
        X -= rhs.X;
        Y -= rhs.Y;
        Z -= rhs.Z;
        return *this;
    }
    Vector3 operator-(const Vector3 &b)
    {
        Vector3 v;
        v.X = this->X - b.X;
        v.Y = this->Y - b.Y;
        v.Z = this->Z - b.Z;
        return v;
    }

    Vector3 &operator*=(float f)
    {
        X *= f;
        Y *= f;
        Z *= f;
        return *this;
    }
    Vector3 operator*(float f)
    {
        Vector3 v;
        v.X = this->X * f;
        v.Y = this->Y * f;
        v.Z = this->Z * f;
        return v;
    }

    Vector3 &operator/=(float f)
    {
        X /= f;
        Y /= f;
        Z /= f;
        return *this;
    }
    Vector3 operator/(float f)
    {
        Vector3 v;
        v.X = this->X / f;
        v.Y = this->Y / f;
        v.Z = this->Z / f;
        return v;
    }
};

#endif

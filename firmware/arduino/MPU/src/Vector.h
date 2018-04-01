#ifndef VECTOR_H
#define VECTOR_H

struct Vector3
{
  float X, Y, Z;

  float module(bool useX, bool useY, bool useZ)
  {
    float result = 0;
    
    if(useX)
    {
      result += pow(X, 2);
    }
    if(useY)
    {
      result += pow(Y, 2);
    }
    if(useZ)
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

  Vector3& operator+=(const Vector3& rhs)
  {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    return *this;
  }
  Vector3& operator-=(const Vector3& rhs)
  {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    return *this;
  }
};

#endif

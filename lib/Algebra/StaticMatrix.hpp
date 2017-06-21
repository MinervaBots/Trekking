#ifndef StaticMatrix_hpp
#define StaticMatrix_hpp

#include "Matrix.hpp"

template<typename T, unsigned char RowsCount, unsigned char ColumnsCount>
class StaticMatrix : public Matrix<T>
{
private:
  T m_Values[RowsCount][ColumnsCount];

public:
  StaticMatrix(T defaultValue) : Matrix<T>::Matrix(defaultValue, RowsCount, ColumnsCount)
  {
  }

  ~StaticMatrix()
  {
    delete [] m_Values;
  }

  Checked<T> get(unsigned char row, unsigned char column) const
  {
    if(Matrix<T>::check(row, column))
    {
      return Checked<T>(true, m_Values[row][column]);
    }
    return Checked<T>(false);
  }

  bool set(unsigned char row, unsigned char column, const T& value)
  {
    if(Matrix<T>::check(row, column))
    {
      m_Values[row][column] = value;
      return true;
    }
    return false;
  }
};

#endif //StaticMatrix_hpp

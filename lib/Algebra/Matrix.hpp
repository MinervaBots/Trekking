#ifndef Matrix_hpp
#define Matrix_hpp

#include "../CustomTypes/Checked.hpp"

template<typename T>
class Matrix
{
public:
  Matrix() : m_Rows(0), m_Columns(0), m_Values(nullptr)
  {
  }

  Matrix(unsigned char rows, unsigned char columns, T defaultValue) : m_Rows(rows), m_Columns(columns)
  {
    m_Values = new T[rows * columns];
    for(unsigned char row = 0; row < m_Rows; row++)
    {
      for(unsigned char column = 0; column < m_Columns; column++)
      {
        set(row, column, defaultValue);
        //m_Values[m_Rows * row + column] = defaultValue;
      }
    }
  }

  ~Matrix()
  {
    /*
    for(unsigned char row = 0; row < m_Rows; row++)
    {
      for(unsigned char column = 0; column < m_Columns; column++)
      {
        delete m_Values[m_Rows * row + column];
      }
    }
    */
    delete [] m_Values;
  }

  Checked<T> get(unsigned char row, unsigned char column) const
  {
    if(check(row, column))
    {
      return Checked<T>(true, m_Values[m_Rows * row + column]);
    }
    return Checked<T>(false);
  }

  bool set(unsigned char row, unsigned char column, const T& value)
  {
    if(check(row, column))
    {
      m_Values[m_Rows * row + column] = value;
      return true;
    }
    return false;
  }

  unsigned char getRowsCount() const { return m_Rows; }
  unsigned char getColumnsCount() const { return m_Columns; }

private:
  unsigned char m_Rows;
  unsigned char m_Columns;
  T *m_Values;

  bool check(unsigned char row, unsigned char column) const
  {
    return m_Values != nullptr && row < m_Rows && column < m_Columns;
  }
};

#endif //Matrix_hpp

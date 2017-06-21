#ifndef Matrix_hpp
#define Matrix_hpp

#include "../CustomTypes/Checked.hpp"

template<typename T>
class Matrix
{
private:
  unsigned char m_RowsCount;
  unsigned char m_ColumnsCount;

protected:
  bool check(unsigned char row, unsigned char column) const
  {
    return row < m_RowsCount && column < m_ColumnsCount;
  }

public:
  Matrix(T defaultValue, unsigned char rowsCount, unsigned char columnsCount)
  {
    m_RowsCount = rowsCount;
    m_ColumnsCount = columnsCount;

    for(unsigned char row = 0; row < rowsCount; row++)
    {
      for(unsigned char column = 0; column < columnsCount; column++)
      {
        set(row, column, defaultValue);
      }
    }
  }

  ~Matrix()
  {
  }

  virtual Checked<T> get(unsigned char row, unsigned char column) const = 0;
  virtual bool set(unsigned char row, unsigned char column, const T& value) = 0;

  unsigned char getRowsCount() const { return m_RowsCount; }
  unsigned char getColumnsCount() const { return m_ColumnsCount; }
};

#endif //Matrix_hpp

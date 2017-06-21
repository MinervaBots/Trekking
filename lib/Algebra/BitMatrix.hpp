#ifndef BitMatrix_hpp
#define BitMatrix_hpp

#include "Matrix.hpp"

#define BitVal(data, y)   ((data >> y) & 1) /** Return Data.Y value  **/
#define SetBit(data, y)   data |= (1 << y)  /** Set Data.Y   to 1    **/
#define ClearBit(data, y) data &= ~(1 << y) /** Clear Data.Y to 0    **/

template<unsigned char RowsCount, unsigned char ColumnsCount>
class BitMatrix : public Matrix<bool>
{
private:
  unsigned char m_RealRowsCount = (unsigned char)ceil(RowsCount / 8);
  unsigned char m_RealColumnsCount = (unsigned char)ceil(RowsCount / 8);
  bool m_Values[(unsigned char)ceil(RowsCount / 8) * (unsigned char)ceil(ColumnsCount / 8)];

public:
  BitMatrix(bool defaultValue) :
    Matrix<bool>::Matrix(defaultValue, m_RealRowsCount, m_RealColumnsCount)
  {
  }

  Checked<bool> get(unsigned char row, unsigned char column)
  {
    if(!check(row, column))
    {
      return Checked<bool>(false);
    }

    unsigned char realRow = (unsigned char)ceil(row / 8);
    unsigned char realColumn = (unsigned char)ceil(column / 8);

    bool value = m_Values[realRow * m_RealRowsCount + realColumn];

    unsigned char bitNumber = column % 8;
    bool bitValue = BitVal(value, bitNumber);
    return Checked<bool>(true, bitValue);
  }

  bool set(unsigned char row, unsigned char column, const bool& value)
  {
    if(!check(row, column))
    {
      return false;
    }

    unsigned char realRow = (unsigned char)ceil(row / 8);
    unsigned char realColumn = (unsigned char)ceil(column / 8);
    unsigned char bitNumber = column % 8;

    if(value)
    {
      SetBit(m_Values[realRow * m_RealRowsCount + realColumn], bitNumber);
    }
    else
    {
      ClearBit(m_Values[realRow * m_RealRowsCount + realColumn], bitNumber);
    }
    return true;
  }
};

#endif //BitMatrix_hpp

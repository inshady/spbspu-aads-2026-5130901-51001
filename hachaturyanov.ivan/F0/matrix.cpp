#include "matrix.hpp"

namespace hachaturyanov
{
  Matrix::Matrix(size_t rows, size_t cols, int fill):
   rows_(rows),
   cols_(cols),
   data_(rows * cols > 0 ? rows * cols : 1)
  {
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        data_.add({ i, j }, fill);
      }
    }
  }

  size_t Matrix::rows() const noexcept
  {
    return rows_;
  }

  size_t Matrix::cols() const noexcept
  {
    return cols_;
  }

  void Matrix::checkBounds_(size_t i, size_t j) const
  {
    if (i >= rows_ || j >= cols_) {
      throw std::out_of_range("Matrix index out of range");
    }
  }

  int Matrix::get(size_t i, size_t j) const
  {
    checkBounds_(i, j);
    return data_.get({ i, j });
  }

  void Matrix::set(size_t i, size_t j, int value)
  {
    checkBounds_(i, j);
    data_.get({ i, j }) = value;
  }
}
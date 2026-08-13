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

  void Matrix::checkSameDims_(const Matrix &other) const
  {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw std::logic_error("Matrix dimensions don't match");
    }
  }

  Matrix Matrix::operator+(const Matrix &other) const
  {
    checkSameDims_(other);
    Matrix result(rows_, cols_, 0);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
      result.set(i, j, get(i, j) + other.get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::operator-(const Matrix &other) const
  {
    checkSameDims_(other);
    Matrix result(rows_, cols_, 0);
    for(size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(i, j, get(i, j) - other.get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::operator*(const Matrix &other) const
  {
    if (cols_ != other.rows_) {
      throw std::logic_error("Matrix dimensions don't match");
    }
    Matrix result(rows_, other.cols_, 0);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t k = 0; k < cols_; k++) {
        int a = get(i, k);
        if (a == 0) {
          continue;
        }
        for (size_t j = 0; j < other.cols_; j++) {
          result.set(i, j, result.get(i, j) + a * other.get(k, j));
        }
      }
    }
    return result;
  }
}
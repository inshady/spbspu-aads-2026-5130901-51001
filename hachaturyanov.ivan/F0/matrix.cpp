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

  Matrix Matrix::scale(int factor) const
  {
    Matrix result(rows_, cols_, 0);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(i, j, factor * get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::transpose() const
  {
    Matrix result(cols_, rows_, 0);
    for(size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(j, i, get(i, j));
      }
    }
    return result;
  }

  bool Matrix::operator==(const Matrix &other) const
  {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      return false;
    }
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (get(i, j) != other.get(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  
  bool Matrix::operator!=(const Matrix &other) const
  {
    return !(*this == other);
  }

  void Matrix::insert_row(size_t index, List< int > &values)
  {
    if (cols_ != values.size()) {
      throw std::logic_error("Numbers of values and cols don't match");
    }
    if (index > rows_) {
      throw std::out_of_range("Index out of range");
    }

    auto it = values.begin();
    Store temp((rows_ + 1) * cols_);

    for (size_t i = 0; i < rows_ + 1; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (i < index) {
          temp.add({ i, j }, get(i, j));
        } else if (i == index) {
          temp.add({ i, j }, *it);
          ++it;
        } else {
          temp.add({ i, j }, get(i - 1, j));
        }
      }
    }
    data_ = std::move(temp);
    rows_++;
  }

  void Matrix::insert_col(size_t index, List< int > &values)
  {
    if (rows_ != values.size()) {
      throw std::logic_error("Numbers of values and rows don't match");
    }
    if (index > cols_) {
      throw std::out_of_range("Index out of range");
    }

    auto it = values.begin();
    Store temp(rows_ * (cols_ + 1));

    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_ + 1; j++) {
        if (j < index) {
          temp.add({ i, j }, get(i, j));
        } else if (j == index) {
          temp.add({ i, j }, *it);
          ++it;
        } else {
          temp.add({ i, j }, get(i, j - 1));
        }
      }
    }
    data_ = std::move(temp);
    cols_++;
  }

  void Matrix::delete_row(size_t index)
  {
    if (index >= rows_) {
      throw std::out_of_range("Index out of range");
    }
    if (rows_ == 1) {
      throw std::logic_error("Matrix contains only one row");
    }

    Store temp((rows_ - 1) * cols_);

    for(size_t i = 0; i < rows_ - 1; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (i < index) {
          temp.add({ i, j }, get(i, j));
        } else {
          temp.add({ i, j }, get(i + 1, j));
        }
      }
    }
    data_ = std::move(temp);
    rows_--;
  }

  void Matrix::delete_col(size_t index)
  {
    if (index >= cols_) {
      throw std::out_of_range("Index out of range");
    }
    if (cols_ == 1) {
      throw std::logic_error("Matrix contains only one col");
    }

    Store temp(rows_ * (cols_ - 1));

    for(size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_ - 1; j++) {
        if (j < index) {
          temp.add({ i, j }, get(i, j));
        } else {
          temp.add({ i, j }, get(i, j + 1));
        }
      }
    }
    data_ = std::move(temp);
    cols_--;
  }

  void Matrix::append_rows(const Matrix &source, size_t start, size_t end)
  {
    if (source.cols_ != cols_) {
      throw std::logic_error("Numbers of cols don't match");
    }
    if (start >= source.rows_ || end >= source.rows_ || start > end) {
      throw std::out_of_range("Index out of range");
    }

    size_t n = end - start + 1;
    Store temp((rows_ + n) * cols_);

    for (size_t i = 0; i < rows_ + n; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (i < rows_) {
          temp.add({ i, j }, get(i, j));
        } else {
          temp.add({ i, j }, source.get(start, j));
          if (j == cols_ - 1) {
            start++;
          }
        }
      }
    }
    data_ = std::move(temp);
    rows_ += n;
  }

  Matrix Matrix::embed(size_t newRows, size_t newCols, size_t rowOffset, size_t colOffset, int fill) const
  {
    if (rowOffset + rows_ > newRows || colOffset + cols_ > newCols) {
      throw std::out_of_range("Index out of range");
    }

    Matrix result(newRows, newCols, fill);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(i + rowOffset, j + colOffset, get(i, j));
      }
    }
    return result;
  }
}
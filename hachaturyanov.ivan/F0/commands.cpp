#include "commands.hpp"

namespace hachaturyanov
{
  void cmdNew(MatrixTable &matrices, std::string name, size_t rows, size_t cols, int fill, std::ostream &out)
  {
    if (!matrices.has(name) && rows && cols) {
      matrices.add(name, Matrix(rows, cols, fill));
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDrop(MatrixTable &matrices, std::string name, std::ostream &out)
  {
    if (matrices.has(name)) {
      matrices.drop(name);
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdShow(const MatrixTable &matrices, std::string name, std::ostream &out)
  {
    if (matrices.has(name)) {
      const Matrix &matrix = matrices.get(name);
      out << "<ROWS: " << matrix.rows() << ", COLS: " << matrix.cols() << ", DATA: [";
      for (size_t i = 0; i < matrix.rows(); i++) {
        out << "[";
        for (size_t j = 0; j < matrix.cols(); j++) {
          out << matrix.get(i, j);
          if (j < matrix.cols() - 1) {
            out << ", ";
          }
        }
        out << "]";
        if (i < matrix.rows() - 1) {
          out << ", ";
        }
      }
      out << "]>" << '\n';
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdSet(MatrixTable &matrices, std::string name, size_t row, size_t col, int value, std::ostream &out)
  {
    try {
      matrices.get(name).set(row, col, value);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdGet(const MatrixTable &matrices, std::string name, size_t row, size_t col, std::ostream &out)
  {
    try {
      int result = matrices.get(name).get(row, col);
      out << result << '\n';
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdAdd(MatrixTable &matrices, const std::string &res,
      const std::string &matrix1, const std::string &matrix2, std::ostream &out)
  {
    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &lhs = matrices.get(matrix1);
      const Matrix &rhs = matrices.get(matrix2);
      Matrix result = lhs + rhs;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdSub(MatrixTable &matrices, const std::string &res,
      const std::string &matrix1, const std::string &matrix2, std::ostream &out)
  {
    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &lhs = matrices.get(matrix1);
      const Matrix &rhs = matrices.get(matrix2);
      Matrix result = lhs - rhs;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdMul(MatrixTable &matrices, const std::string &res,
      const std::string &matrix1, const std::string &matrix2, std::ostream &out)
  {
    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &lhs = matrices.get(matrix1);
      const Matrix &rhs = matrices.get(matrix2);
      Matrix result = lhs * rhs;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdScale(MatrixTable &matrices, const std::string &res,
      const std::string &name, int scalar, std::ostream &out)
  {
    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &matrix = matrices.get(name);
      Matrix result = matrix * scalar;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdTranspose(MatrixTable &matrices, std::string res, std::string name, std::ostream &out)
  {
    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &matrix = matrices.get(name);
      Matrix result = matrix.transpose();

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdInsertRow(MatrixTable &matrices, const std::string &name, 
      size_t rowIndex, const List< int > &values, std::ostream &out)
  {
    try {
      Matrix &matrix = matrices.get(name);
      matrix.insert_row(rowIndex, values);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }
}
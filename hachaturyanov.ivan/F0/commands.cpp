#include "commands.hpp"

namespace hachaturyanov
{
  List< std::string > tokenize(const std::string &line)
  {
    List< std::string > tokens;
    size_t pos = 0;

    while (pos < line.size()) {
      while (pos < line.size() && std::isspace(line[pos])) {
        pos++;
      }
      if (pos >= line.size()) {
        break;
      }
      size_t start = pos;
      while (pos < line.size() && !std::isspace(line[pos])) {
        pos++;
      }
      tokens.addEnd(line.substr(start, pos - start));
    }
    return tokens;
  }

  void cmdNew(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t rows = std::stoul(*it);
    ++it;
    size_t cols = std::stoul(*it);
    ++it;
    int fill = std::stoi(*it);

    if (!matrices.has(name) && rows && cols) {
      matrices.add(name, Matrix(rows, cols, fill));
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDrop(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    auto it = tokens.begin();
    ++it;
    std::string name = *it;

    if (matrices.has(name)) {
      matrices.drop(name);
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdShow(const List< std::string > &tokens, const MatrixTable &matrices, std::ostream &out)
  {
    auto it = tokens.begin();
    ++it;
    std::string name = *it;

    if (tokens.size() == 2 || matrices.has(name)) {
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

  void cmdSet(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t row = std::stoul(*it);
    ++it;
    size_t col = std::stoul(*it);
    ++it;
    int value = std::stoi(*it);

    try {
      matrices.get(name).set(row, col, value);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdGet(const List< std::string > &tokens, const MatrixTable &matrices, std::ostream &out)
  {
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t row = std::stoul(*it);
    ++it;
    size_t col = std::stoul(*it);
    ++it;

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

  void cmdInsertCol(MatrixTable &matrices, const std::string &name,
      size_t colIndex, const List< int > &values, std::ostream &out)
  {
    try {
      Matrix &matrix = matrices.get(name);
      matrix.insert_col(colIndex, values);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDeleteRow(MatrixTable &matrices, const std::string &name, size_t rowIndex, std::ostream &out)
  {
    try {
      Matrix &matrix = matrices.get(name);
      matrix.delete_row(rowIndex);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDeleteCol(MatrixTable &matrices, const std::string &name, size_t colIndex, std::ostream &out)
  {
    try {
      Matrix &matrix = matrices.get(name);
      matrix.delete_col(colIndex);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdAppendRows(MatrixTable &matrices, const std::string &source,
      const std::string &dest, size_t rowStart, size_t rowEnd, std::ostream &out)
  {
    try {
      const Matrix &sourceMatrix = matrices.get(source);
      Matrix &destMatrix = matrices.get(dest);

      destMatrix.append_rows(sourceMatrix, rowStart, rowEnd);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdEmbed(MatrixTable &matrices, const std::string &result, const std::string &source,
      size_t rows, size_t cols, size_t rowOffset, size_t colOffset, int fill, std::ostream &out)
  {
    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.embed(rows, cols, rowOffset, colOffset, fill);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdJoinRight(MatrixTable &matrices, const std::string &result,
      const std::string &dest, const std::string &source, int fill, std::ostream &out)
  {
    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = destMatrix.join_right(sourceMatrix, fill);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdJoinBottom(MatrixTable &matrices, const std::string &result,
      const std::string &dest, const std::string &source, int fill, std::ostream &out)
  {
    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = destMatrix.join_bottom(sourceMatrix, fill);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdCrop(MatrixTable &matrices, const std::string &result, const std::string &source,
      size_t row, size_t col, size_t rows, size_t cols, std::ostream &out)
  {
    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.crop(row, col, rows, cols);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdReplace(MatrixTable &matrices, const std::string &dest,
      const std::string &source, size_t row, size_t col, std::ostream &out)
  {
    try {
      Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);

      destMatrix.replace(sourceMatrix, row, col);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdFlatten(MatrixTable &matrices, const std::string &result, const std::string &source, std::ostream &out)
  {
    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.flatten();

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdRepeat(MatrixTable &matrices, const std::string &result, const std::string &source,
      size_t tileRows, size_t tileCols, std::ostream &out)
  {
    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.repeat(tileRows, tileCols);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }


}

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "../common/hash_table_robin.hpp"
#include "../common/io_utils.hpp"
#include "matrix.hpp"

namespace hachaturyanov
{
  using MatrixTable = HashTable< std::string, Matrix, std::hash< std::string >, std::equal_to< std::string > >;

  void cmdNew(MatrixTable &matrices, const std::string &name, size_t rows, size_t cols, int fill, std::ostream &out);
  void cmdDrop(MatrixTable &matrices, const std::string &name, std::ostream &out);
  void cmdShow(const MatrixTable &matrices, const std::string &name, std::ostream &out);
  void cmdSet(MatrixTable &matrices, const std::string &name, size_t row, size_t col, int value, std::ostream &out);
  void cmdGet(const MatrixTable &matrices, const std::string &name, size_t row, size_t col, std::ostream &out);
  void cmdAdd(MatrixTable &matrices, const std::string &res,
      const std::string &matrix1, const std::string &matrix2, std::ostream &out);
  void cmdSub(MatrixTable &matrices, const std::string &res,
      const std::string &matrix1, const std::string &matrix2, std::ostream &out);
  void cmdMul(MatrixTable &matrices, const std::string &res,
      const std::string &matrix1, const std::string &matrix2, std::ostream &out);
  void cmdScale(MatrixTable &matrices, const std::string &res,
      const std::string &name, int scalar, std::ostream &out);
  void cmdTranspose(MatrixTable &matrices, const std::string &res, const std::string &name, std::ostream &out);
  void cmdInsertRow(MatrixTable &matrices, const std::string &name, 
      size_t rowIndex, const List< int > &values, std::ostream &out);
}

#endif
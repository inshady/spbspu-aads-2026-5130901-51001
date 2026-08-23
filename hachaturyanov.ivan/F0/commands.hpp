#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "../common/hash_table_robin.hpp"
#include "../common/io_utils.hpp"
#include "matrix.hpp"

namespace hachaturyanov
{
  using MatrixTable = HashTable< std::string, Matrix, std::hash< std::string >, std::equal_to< std::string > >;

  List< std::string > tokenize(const std::string &line);
  void cmdNew(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdDrop(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdShow(const List< std::string > &tokens, const MatrixTable &matrices, std::ostream &out);
  void cmdSet(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdGet(const List< std::string > &tokens, const MatrixTable &matrices, std::ostream &out);
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
  void cmdInsertCol(MatrixTable &matrices, const std::string &name,
      size_t colIndex, const List< int > &values, std::ostream &out);
  void cmdDeleteRow(MatrixTable &matrices, const std::string &name, size_t rowIndex, std::ostream &out);
  void cmdDeleteCol(MatrixTable &matrices, const std::string &name, size_t colIndex, std::ostream &out);
  void cmdAppendRows(MatrixTable &matrices, const std::string &source,
      const std::string &dest, size_t rowStart, size_t rowEnd, std::ostream &out);
  void cmdEmbed(MatrixTable &matrices, const std::string &result, const std::string &source,
      size_t rows, size_t cols, size_t rowOffset, size_t colOffset, int fill, std::ostream &out);
  void cmdJoinRight(MatrixTable &matrices, const std::string &result,
      const std::string &dest, const std::string &source, int fill, std::ostream &out);
  void cmdJoinBottom(MatrixTable &matrices, const std::string &result,
      const std::string &dest, const std::string &source, int fill, std::ostream &out);
  void cmdCrop(MatrixTable &matrices, const std::string &result, const std::string &source,
      size_t row, size_t col, size_t rows, size_t cols, std::ostream &out);
  void cmdReplace(MatrixTable &matrices, const std::string &dest,
      const std::string &source, size_t row, size_t col, std::ostream &out);
  void cmdFlatten(MatrixTable &matrices, const std::string &result, const std::string &source, std::ostream &out);
  void cmdRepeat(MatrixTable &matrices, const std::string &result, const std::string &source,
      size_t tileRows, size_t tileCols, std::ostream &out);
}

#endif

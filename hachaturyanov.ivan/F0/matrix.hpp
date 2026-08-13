#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include "../common/hash_table_robin.hpp"

namespace hachaturyanov
{
  struct PairHash {
    size_t operator()(const std::pair< size_t, size_t > &p) const noexcept
    {
      size_t h1 = std::hash< size_t >{}(p.first);
      size_t h2 = std::hash< size_t >{}(p.second);
      return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
  };

  struct PairEqual {
    bool operator()(const std::pair< size_t, size_t > &a,
        const std::pair< size_t, size_t > &b) const noexcept
    {
      return a.first == b.first && a.second == b.second;
    }
  };

  class Matrix {
   public:
    using Key = std::pair< size_t, size_t >;
    using Store = HashTable< Key, int, PairHash, PairEqual >;

    Matrix(size_t rows, size_t cols, int fill);

    size_t rows() const noexcept;
    size_t cols() const noexcept;

    int get(size_t i, size_t j) const;
    void set(size_t i, size_t j, int value);

   private:
    size_t rows_;
    size_t cols_;
    Store data_;

    void checkBounds_(size_t i, size_t j) const;
  };
}

#endif
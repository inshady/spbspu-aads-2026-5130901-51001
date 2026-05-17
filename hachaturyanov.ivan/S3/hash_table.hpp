#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../common/list.hpp"

namespace hachaturyanov
{
  enum State {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template< class Key, class Value > struct Slot {
    Key key;
    Value value;
    State state = EMPTY;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
   public:
    HashTable();
    HashTable(size_t slots);

    void add(const Key &key, const Value &value);
    Value drop(const Key &key);
    bool has(const Key &key) const;
    void rehash(size_t slots);
   private:
    List< Slot< Key, Value > > *data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
   data_(nullptr),
   size_(0),
   capacity_(0),
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
   data_(new List< Slot< Key, Value > >[slots]),
   size_(0),
   capacity_(slots)
  {}

}
#endif

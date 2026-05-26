#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../common/list.hpp"

namespace hachaturyanov
{
  static constexpr size_t DEFAULT_CAPACITY = 17;

  inline bool isPrime(size_t n) {
    if (n <= 1) {
      return false;
    }
    if (n <= 3) {
      return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
      return false;
    }
    for (size_t i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0) {
        return false;
      }
    }
    return true;
  }

  inline size_t nextPrime(size_t n) {
    if (n <= 2) {
      return 2;
    }
    if (n % 2 == 0) {
      n++;
    } else {
      n += 2;
    }
    while (!isPrime(n)) {
      n += 2;
    }
    return n;
  }

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
    Slot< Key, Value >* data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
   data_(new Slot< Key, Value >[DEFAULT_CAPACITY]),
   size_(0),
   capacity_(DEFAULT_CAPACITY)
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
   data_(new Slot< Key, Value >[slots]),
   size_(0),
   capacity_(nextPrime(slots))
  {}

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key &key, const Value &value)
  {
    bool added = false;
    bool tombstone_found = false;
    size_t tombstone_id = 0;
    for (size_t i = 0; i < capacity_; i++) {
      size_t id = (hash_(key) + i * i) % capacity_;
      Slot< Key, Value > &slot = data_[id];
      if (slot.state == EMPTY) {
        if (tombstone_found) {
          slot = data_[tombstone_id];
        }
        slot.key = key;
        slot.value = value;
        slot.state = OCCUPIED;
        size_++;
        added = true;
        break;
      } else if (slot.state == TOMBSTONE && !tombstone_found) {
        tombstone_found = true;
        tombstone_id = id;
      } else if (slot.state == OCCUPIED && equal_(slot.key, key)) {
        slot.value = value;
        break;
      }
    }

    if (!added) {
      throw std::runtime_error("Failed to add key-value pair, rehashing recommended");
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key &key)
  {
    for (size_t i = 0; i < capacity_; i++) {
      size_t id = (hash_(key) + i * i) % capacity_;
      Slot< Key, Value > &slot = data_[id];
      if (slot.state == EMPTY) {
        break;
      } else if (slot.state == OCCUPIED && equal_(slot.key, key)) {
        Value value = slot.value;
        slot.state = TOMBSTONE;
        size_--;
        return value;
      }
    }
    throw std::runtime_error("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key &key) const
  {
    for (size_t i = 0; i < capacity_; i++) {
      size_t id = (hash_(key) + i * i) % capacity_;
      Slot< Key, Value > &slot = data_[id];
      if (slot.state == EMPTY) {
        break;
      } else if (slot.state == OCCUPIED && equal_(slot.key, key)) {
        return true;
      }
    }
    return false;
  }
}
#endif

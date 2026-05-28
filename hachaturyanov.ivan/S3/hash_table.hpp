#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP


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
    ~HashTable();

    void add(const Key &key, const Value &value);
    Value drop(const Key &key);
    bool has(const Key &key) const;
    void rehash(size_t slots);

    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();

   private:
    Slot< Key, Value >* data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;
    void swap(HashTable &other);
  };

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::capacity() const
  {
    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    for (size_t i = 0; i < capacity_; i++) {
      data_[i].state = EMPTY;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable &other)
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(hash_, other.hash_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
   data_(new Slot< Key, Value >[nextPrime(DEFAULT_CAPACITY)]),
   size_(0),
   capacity_(nextPrime(DEFAULT_CAPACITY))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
   data_(new Slot< Key, Value >[nextPrime(slots)]),
   size_(0),
   capacity_(nextPrime(slots))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
  }

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
        size_t target = tombstone_found ? tombstone_id : id;
        data_[target].key = key;
        data_[target].value = value;
        data_[target].state = OCCUPIED;
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
      const Slot< Key, Value > &slot = data_[id];
      if (slot.state == EMPTY) {
        break;
      } else if (slot.state == OCCUPIED && equal_(slot.key, key)) {
        return true;
      }
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    size_t new_capacity = nextPrime(slots);
    Slot< Key, Value >* new_data = new Slot< Key, Value >[new_capacity];

    for (size_t i = 0; i < capacity_; i++) {
      if (data_[i].state == OCCUPIED) {
        for (size_t j = 0; j < new_capacity; j++) {
          size_t id = (hash_(data_[i].key) + j * j) % new_capacity;
          if (new_data[id].state == EMPTY) {
            new_data[id] = data_[i];
            break;
          }
        }
      }
    }

    std::swap(data_, new_data);
    std::swap(capacity_, new_capacity);
    delete[] new_data;
  }

}
#endif

#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <utility>
#include <functional>

namespace hachaturyanov
{
  template< class Key, class Value > struct BSTNode {
    std::pair< const Key, Value > data;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
  };

  template< class Key, class Value, class Compare > class BSTree;
  template< class Key, class Value > class BSTIterator;

  template< class Key, class Value > class BSTConstIterator {
    using Node = BSTNode< Key, Value >;
    template< class K, class V, class C > friend class BSTree;
    friend class BSTIterator< Key, Value >;

    Node* node_;
    Node* fake_;

    BSTConstIterator(Node* node, Node* fake);

   public:
    using value_type = std::pair< const Key, Value >;

    const value_type & operator*() const;
    const value_type* operator->() const;
    BSTConstIterator & operator++();

    bool operator==(const BSTConstIterator &other) const;
    bool operator!=(const BSTConstIterator &other) const;

    BSTConstIterator(const BSTIterator< Key, Value > &other);
  };

  template< class Key, class Value > class BSTIterator {
    using Node = BSTNode< Key, Value >;
    template< class K, class V, class C > friend class BSTree;

    Node* node_;
    Node* fake_;

    BSTIterator(Node* node, Node* fake);

   public:
    using value_type = std::pair< const Key, Value >;

    value_type & operator*() const;
    value_type* operator->() const;
    BSTIterator & operator++();

    bool operator==(const BSTIterator &other) const;
    bool operator!=(const BSTIterator &other) const;
  };

  template< class Key, class Value, class Compare > class BSTree {
    using Node = BSTNode< Key, Value >;

    Node* root_;
    Node* fake_;

    Node* makeNode(const Key &key, const Value &value);
    Node* makeFake();
    size_t height_(Node* n) const;

   public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    ~BSTree();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool isEmpty() const;

    void push(const Key &key, const Value &value);
    Value get(const Key &key) const;
    Value drop(const Key &key);

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

    void clear();
  };

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator &other) const
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator &other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator++()
  {
    if (node_->right != fake_) {
      node_ = node_->right;
      while (node_->left != fake_) {
        node_ = node_->left;
      }
    } else {
      Node* par = node_->parent;
      while (par != nullptr && node_ == par->right) {
        node_ = par;
        par = par->parent;
      }
      node_ = par;
    }
    return *this;
  }

  template< class Key, class Value >
  typename BSTIterator< Key, Value >::value_type* BSTIterator< Key, Value >::operator->() const
  {
    return &node_->data;
  }

  template< class Key, class Value >
  typename BSTIterator< Key, Value >::value_type & BSTIterator< Key, Value >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(Node* node, Node* fake):
   node_(node),
   fake_(fake)
  {}

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator & other) const
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTConstIterator & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > & BSTConstIterator< Key, Value >::operator++()
  {
    if (node_->right != fake_) {
      node_ = node_->right;
      while (node_->left != fake_) {
        node_ = node_ ->left;
      }
    } else {
      Node* par = node_->parent;
      while (par != nullptr && node_ == par->right) {
        node_ = par;
        par = par->parent;
      }
      node_ = par;
    }
    return *this;
  }

  template< class Key, class Value >
  const typename BSTConstIterator< Key, Value >::value_type*
      BSTConstIterator< Key, Value >::operator->() const
  {
    return &node_->data;
  }

  template< class Key, class Value >
  const typename BSTConstIterator< Key, Value >::value_type &
      BSTConstIterator< Key, Value >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(Node* node, Node* fake):
   node_(node),
   fake_(fake)
  {}
}

#endif

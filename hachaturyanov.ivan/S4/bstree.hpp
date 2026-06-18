#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <utility>

namespace hachaturyanov
{
  template< class Key, class Value > struct BSTNode {
    std::pair< const Key, Value > data;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
  };

  template< class Key, class Value, class Compare > class BSTree;

  template< class Key, class Value > class BSTConstIterator {
    using Node = BSTNode< Key, Value >;
    template< class K, class V, class C > friend class BSTree;

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
  };

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

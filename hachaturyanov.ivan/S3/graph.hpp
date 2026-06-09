#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"
#include "xxhash.hpp"
#include "../common/list.hpp"

namespace hachaturyanov
{
  using graph_key = std::pair< std::string, std::string >;
  class Graph {
    HashTable< graph_key, List< size_t >, xxhash< graph_key >, std::equal_to< graph_key > > links_;
    List< std::string >* vertices_;
    size_t links_count_;
    size_t vertices_count_;

    void swap(Graph &other) noexcept;
    template< class T > void insertSorted(List< T >* list, const T &val);
   public:
    Graph();
    Graph(const Graph &other);
    Graph(Graph &&other) noexcept;
    Graph &operator=(const Graph &other);
    Graph &operator=(Graph &&other) noexcept;

    Graph(size_t links_count,
      List< std::pair< std::pair< std::string, std::string >, size_t > >* links_list);

    const List< std::string >* getVertices() const noexcept;
    List< std::pair< std::string, List< size_t >* > >* getOutboundLinks(std::string vertex) const;
  };

}

#endif

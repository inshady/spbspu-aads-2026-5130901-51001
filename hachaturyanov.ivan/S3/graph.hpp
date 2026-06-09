#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"
#include "xxhash.hpp"
#include "../common/list.hpp"

namespace hachaturyanov
{
  using vertices_pair = std::pair< std::string, std::string >;
  using new_links_list = List< std::pair< vertices_pair, size_t > >;
  using oneway_links_list = List< std::pair< std::string, List< size_t >* > >;

  using graph_table = HashTable< vertices_pair, List< size_t >*,
        xxhash< vertices_pair >, std::equal_to< vertices_pair > >;

  class Graph {
    graph_table links_;
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

    Graph(size_t links_count, new_links_list* links_list);

    const List< std::string >* getVertices() const noexcept;
    oneway_links_list* getLinks(const std::string &vertex, bool outbound) const;
    void bind(const std::string &vertex1, const std::string &vertex2, size_t weight);
  };

}

#endif

#include "graph.hpp"

namespace hachaturyanov
{
  Graph::Graph():
   name_(""),
   links_(),
   vertices_(new List< std::string >()),
   links_count_(0),
   vertices_count_(0)
  {}

  Graph::Graph(const Graph &other):
   name_(other.name_),
   links_(HashTable< graph_key, List< size_t >, xxhash< graph_key >, std::equal_to< graph_key > >(other.links_)),
   vertices_(new List< std::string >(*other.vertices_)),
   links_count_(other.links_count_),
   vertices_count_(other.vertices_count_)
  {}

  Graph::Graph(Graph &&other) noexcept:
   name_(std::move(other.name_)),
   links_(std::move(other.links_)),
   vertices_(other.vertices_),
   links_count_(other.links_count_),
   vertices_count_(other.vertices_count_)
  {
    other.vertices_ = nullptr;
    other.links_count_ = 0;
    other.vertices_count_ = 0;
  }

  void Graph::swap(Graph &other) noexcept
  {
    std::swap(name_, other.name_);
    std::swap(links_, other.links_);
    std::swap(vertices_, other.vertices_);
    std::swap(links_count_, other.links_count_);
    std::swap(vertices_count_, other.vertices_count_);
  }

  Graph &Graph::operator=(const Graph &other)
  {
    if (this != &other) {
      Graph copy(other);
      swap(copy);
    }
    return *this;
  }

  Graph &Graph::operator=(Graph &&other) noexcept
  {
    if (this != &other) {
      swap(other);
    }
    return *this;
  }

  Graph::Graph(std::string name, size_t links_count,
    List< std::pair< std::pair< std::string, std::string >, size_t > >* links_list):
   name_(std::move(name)),
   links_(),
   vertices_(nullptr),
   links_count_(links_count),
   vertices_count_(0)
  {
    if (links_list) {
      vertices_ = new List< std::string >();
      auto start = links_list->begin();
      auto it = start;
      do {
        std::pair< std::pair< std::string, std::string >, size_t > data = *it;
        if (links_.has(data.first)) {
          links_[data.first].addEnd(data.second);
        } else {
          links_.add(data.first, List< size_t >{data.second});
        }
        if (!vertices_->has(data.first.first)) {
          vertices_->addEnd(data.first.first);
          vertices_count_++;
        }
        if (!vertices_->has(data.first.second)) {
          vertices_->addEnd(data.first.second);
          vertices_count_++;
        }
        ++it;
      } while (it != start);
    }
  }


}

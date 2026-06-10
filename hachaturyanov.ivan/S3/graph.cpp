#include "graph.hpp"

namespace hachaturyanov
{
  Graph::Graph():
   links_(),
   vertices_(new List< std::string >()),
   links_count_(0),
   vertices_count_(0)
  {}

  Graph::Graph(const Graph &other):
   links_(graph_table(other.links_)),
   vertices_(new List< std::string >(*other.vertices_)),
   links_count_(other.links_count_),
   vertices_count_(other.vertices_count_)
  {}

  Graph::Graph(Graph &&other) noexcept:
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

  template< class T > void Graph::insertSorted(List< T >* list, const T &val)
  {
    if (list->isEmpty()) {
      list->addEnd(val);
      return;
    }
    auto start = list->begin();
    auto it = start;
    do {
      ++it;
    } while (it != start && *it < val);
    list->insertBefore(it, val);
  }

  Graph::Graph(size_t links_count, new_links_list* links_list):
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
          insertSorted< size_t >(&links_[data.first], data.second);
        } else {
          links_.add(data.first, List< size_t >{data.second});
        }
        if (!vertices_->has(data.first.first)) {
          insertSorted< std::string >(vertices_, data.first.first);
          vertices_count_++;
        }
        if (!vertices_->has(data.first.second)) {
          insertSorted< std::string >(vertices_, data.first.second);
          vertices_count_++;
        }
        ++it;
      } while (it != start);
    }
  }

  const List< std::string >* Graph::getVertices() const noexcept
  {
    return vertices_;
  }

  oneway_links_list* Graph::getLinks(const std::string& vertex, bool outbound) const
  {
    if (vertices_->has(vertex)) {
      oneway_links_list* result = new oneway_links_list();
      auto start = vertices_->begin();
      auto it = start;
      do {
        std::string otherVertex = *it;
        vertices_pair verticesPair = {otherVertex, vertex};
        if (outbound) {
          verticesPair = {vertex, otherVertex};
        }
        if (links_.has(verticesPair)) {
          result->addEnd({otherVertex, &links_.get(verticesPair)});
        }
        ++it;
      } while (it != start);
      return result;
    } else {
      throw std::runtime_error("Vertex not found");
    }
  }

  void Graph::bind(const std::string &vertex1, const std::string &vertex2, size_t weight)
  {
    Graph temp(*this);
    vertices_pair verticesPair = {vertex1, vertex2};
    if (temp.links_.has(verticesPair)) {
      insertSorted< size_t >(&temp.links_[verticesPair], weight);
    } else {
      temp.links_.add(verticesPair, List< size_t >{weight});
    }
    if (!temp.vertices_->has(vertex1)) {
      insertSorted< std::string >(temp.vertices_, vertex1);
      temp.vertices_count_++;
    }
    if (!temp.vertices_->has(vertex2)) {
      insertSorted< std::string >(temp.vertices_, vertex2);
      temp.vertices_count_++;
    }
    temp.links_count_++;

    swap(temp);
  }

  void Graph::cut(const std::string &vertex1, const std::string &vertex2, size_t weight)
  {
    Graph temp(*this);
    vertices_pair verticesPair = {vertex1, vertex2};
    if (!temp.vertices_->has(vertex1) || !temp.vertices_->has(vertex2)) {
      throw std::runtime_error("Vertex not found");
    }
    if (!temp.links_.get(verticesPair).has(weight)) {
      throw std::runtime_error("Link not found");
    }
    temp.links_.get(verticesPair).erase(temp.links_.get(verticesPair).find(weight));
    temp.links_count_--;

    swap(temp);
  }

  Graph::Graph(size_t vertices_count, List< std::string >* vertices):
   links_(),
   vertices_(new List< std::string >(*vertices)),
   links_count_(0),
   vertices_count_(vertices_count)
  {}

  Graph::Graph(const Graph &graph1, const Graph &graph2):
   Graph(graph1)
  {
    if (!graph2.vertices_->isEmpty()) {
      auto start = graph2.vertices_->begin();
      auto it = start;
      do {
        std::string vertex1 = *it;
        if (!vertices_->has(vertex1)) {
          insertSorted< std::string >(vertices_, vertex1);
          vertices_count_++;
        }
        oneway_links_list* links = graph2.getLinks(vertex1, true);
        auto linksIt = links->begin();
        do {
          std::string vertex2 = linksIt->first;
          List< size_t >* weights = linksIt->second;
          auto weightsIt = weights->begin();
          do {
            size_t weight = *weightsIt;
            bind(vertex1, vertex2, weight);
            ++weightsIt;
          } while (weightsIt != weights->begin());
          ++linksIt;
        } while (linksIt != links->begin());
        ++it;
        links->clear();
      } while (it != start);
    }
  }

  Graph::Graph(const Graph &other, size_t vertices_count, List< std::string >* vertices):
   links_(),
   vertices_(new List< std::string >(*vertices)),
   links_count_(0),
   vertices_count_(vertices_count)
  {
    auto it = vertices_->begin();
    do {
      std::string vertex1 = *it;
      if (other.vertices_->has(vertex1)) {
        oneway_links_list* links = other.getLinks(vertex1, true);
        auto linksIt = links->begin();
        do {
          std::string vertex2 = linksIt->first;
          if (vertices_->has(vertex2)) {
            List< size_t >* weights = linksIt->second;
            auto weightsIt = weights->begin();
            do {
              size_t weight = *weightsIt;
              bind(vertex1, vertex2, weight);
              ++weightsIt;
            } while (weightsIt != weights->begin());
          }
          ++linksIt;
        } while (linksIt != links->begin());
        delete links;
      } else {
        throw std::runtime_error("Vertex not found");
      }
      ++it;
    } while (it != vertices_->begin());
  }

  Graph::~Graph()
  {
    delete vertices_;
  }
}

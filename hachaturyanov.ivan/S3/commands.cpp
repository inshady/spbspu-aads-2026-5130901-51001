#include <iostream>
#include "graph.hpp"

namespace hachaturyanov
{
  using GraphTable = HashTable< std::string, Graph,
        xxhash< std::string >, std::equal_to< std::string > >;

  void cmdGraphs(const GraphTable &graphs, std::ostream &out)
  {
    List< std::string >* graphNames = graphs.keys();
    auto it = graphNames->begin();
    do {
      out << *it << '\n';
      ++it;
    } while (it != graphNames->begin());
    delete graphNames;
  }

  void cmdVertexes(const GraphTable &graphs, const std::string &graphName, std::ostream &out)
  {
    if (graphs.has(graphName)) {
      const List< std::string > * vertexes = graphs.get(graphName).getVertices();

      auto it = vertexes->begin();
      do {
        out << *it << '\n';
        ++it;
      } while (it != vertexes->begin());
    } else {
      throw std::logic_error("Incorrect graph name");
    }
  }

  void cmdInOutbound(const GraphTable &graphs, const std::string &graphName,
        const std::string &vertexName, bool outbound, std::ostream &out)
  {
    if (graphs.has(graphName)) {
      Graph &graph = graphs.get(graphName);
      if (graph.getVertices()->has(vertexName)) {
        oneway_links_list* links = graph.getLinks(vertexName, outbound);
        if (!links->isEmpty()) {
          auto it = links->begin();
          do {
            std::pair< std::string, List< size_t >* > verLinks = *it;
            out << verLinks.first;
            if (!verLinks.second->isEmpty()) {
              auto linksIt = verLinks.second->begin();
              do {
                out << *linksIt;
                ++linksIt;
              } while (linksIt != verLinks.second->begin());
            }
            out << '\n';
            ++it;
          } while (it != links->begin());
        }
      } else {
        throw std::logic_error("Incorrect vertex name");
      }
    } else {
      throw std::logic_error("Incorrect graph name");
    }
  }

  void cmdBind(GraphTable &graphs, const std::string &vertex1,
        const std::string &vertex2, std::ostream out);
}

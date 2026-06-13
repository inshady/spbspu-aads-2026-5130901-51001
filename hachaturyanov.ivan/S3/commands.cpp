#include "commands.hpp"

namespace hachaturyanov
{

  void readFile(std::istream &in, GraphTable &graphs)
  {
    std::string line = "";
    std::string current_graph = "";
    while(std::getline(in, line))
    {
      List< std::string > strs = split(line);
      if (strs.size() == 2) {
        auto it = strs.begin();
        current_graph = *it;
        ++it;
        graphs[current_graph] = Graph();
      } else if (strs.size() == 3) {
        auto it = strs.begin();
        std::pair< std::string, std::string > vertices = {};
        vertices.first = *it;
        ++it;
        vertices.second = *it;
        ++it;
        graphs[current_graph].bind(vertices.first, vertices.second, std::stoi(*it));
      } else if (!strs.size()) {
        continue;
      }
    }
  }

  void cmdGraphs(const GraphTable &graphs, std::ostream &out)
  {
    List< std::string > graphNames = graphs.keys();
    auto it = graphNames.begin();
    do {
      out << *it << '\n';
      ++it;
    } while (it != graphNames.begin());
    graphNames.clear();
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
      out << "<INVALID COMMAND>" << '\n';
      throw std::logic_error("Incorrect graph name");
    }
  }

  void cmdInOutbound(const GraphTable &graphs, const std::string &graphName,
        const std::string &vertexName, bool outbound, std::ostream &out)
  {
    if (graphs.has(graphName)) {
      const Graph &graph = graphs.get(graphName);
      if (graph.getVertices()->has(vertexName)) {
        oneway_links_list links = graph.getLinks(vertexName, outbound);
        if (!links.isEmpty()) {
          auto it = links.begin();
          do {
            std::pair< std::string, List< size_t > > verLinks = *it;
            out << verLinks.first;
            if (!verLinks.second.isEmpty()) {
              auto linksIt = verLinks.second.begin();
              do {
                out << *linksIt;
                ++linksIt;
              } while (linksIt != verLinks.second.begin());
            }
            out << '\n';
            ++it;
          } while (it != links.begin());
        }
      } else {
        out << "<INVALID COMMAND>" << '\n';
        throw std::logic_error("Incorrect vertex name");
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
      throw std::logic_error("Incorrect graph name");
    }
  }

  void cmdBind(GraphTable &graphs, const std::string &graph, const std::string &vertex1,
        const std::string &vertex2, size_t weight, std::ostream &out)
  {
    if (graphs.has(graph)) {
      graphs[graph].bind(vertex1, vertex2, weight);
    } else {
      out << "<INVALID COMMAND>" << '\n';
      throw std::logic_error("Incorrect graph name");
    }
  }

  void cmdCut(GraphTable &graphs, const std::string &graph, const std::string &vertex1,
        const std::string &vertex2, size_t weight, std::ostream &out)
  {
    if (graphs.has(graph)) {
      graphs[graph].cut(vertex1, vertex2, weight);
    } else {
      out << "<INVALID COMMAND>" << '\n';
      throw std::logic_error("Incorrect graph name");
    }
  }

  void cmdCreate(GraphTable &graphs, const std::string &graph, size_t n,
        List< std::string > &vertices, std::ostream &out)
  {
    if (!graphs.has(graph)) {
      graphs[graph] = Graph(n, &vertices);
    } else {
      out << "<INVALID COMMAND>" << '\n';
      throw std::logic_error("Graph already exists");
    }
  }

  void cmdMerge(GraphTable &graphs, std::string &newgraph,
        std::string &graph1, std::string &graph2, std::ostream &out)
  {
    if (!graphs.has(newgraph)) {
      if (graphs.has(graph1) && graphs.has(graph2)) {
        graphs[newgraph] = Graph(graphs[graph1], graphs[graph2]);
      } else {
        out << "<INVALID COMMAND>" << '\n';
        throw std::logic_error("One of two old graphs doesn't exist");
      }
    } else {
      out << "<INVALID COMMAND" << '\n';
      throw std::logic_error("New graph already exists");
    }
  }

  void cmdExtract(GraphTable &graphs, std::string &newgraph, std::string &oldgraph,
        size_t n, List< std::string > &vertices, std::ostream &out)
  {
    if (!graphs.has(newgraph)) {
      if (graphs.has(oldgraph)) {
        graphs[newgraph] = Graph(graphs[oldgraph], n, &vertices);
      } else {
        out << "<INVALID COMMAND>" << '\n';
        throw std::logic_error("Old graph doesn't exist");
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
      throw std::logic_error("New graph already exists");
    }
  }

  void process(std::istream &in, std::ostream &out, GraphTable &graphs)
  {
    std::string line = "";
    while(std::getline(in, line)) {
      List< std::string > strs = split(line);
      auto it = strs.begin();
      if (*it == "graphs") {
        cmdGraphs(graphs, out);
      } else if (*it == "vertexes") {
        ++it;
        cmdVertexes(graphs, *it, out);
      } else if (*it == "outbound") {
        ++it;
        std::string graphName = *it;
        ++it;
        cmdInOutbound(graphs, graphName, *it, true, out);
      } else if (*it == "inbound") {
        ++it;
        std::string graphName = *it;
        ++it;
        cmdInOutbound(graphs, graphName, *it, false, out);
      } else if (*it == "bind") {
        ++it;
        std::string graphName = *it;
        ++it;
        std::string vertex1 = *it;
        ++it;
        std::string vertex2 = *it;
        ++it;
        size_t weight = stoi(*it);
        cmdBind(graphs, graphName, vertex1, vertex2, weight, out);
      } else if (*it == "cut") {
        ++it;
        std::string graphName = *it;
        ++it;
        std::string vertex1 = *it;
        ++it;
        std::string vertex2 = *it;
        ++it;
        size_t weight = stoi(*it);
        cmdCut(graphs, graphName, vertex1, vertex2, weight, out);
      } else if (*it == "create") {
        ++it;
        std::string graphName = *it;
        ++it;
        size_t n = stoi(*it);
        List< std::string > vertices;
        for (size_t i = 0; i < n; i++) {
          ++it;
          vertices.insertSorted(*it);
        }
        cmdCreate(graphs, graphName, n, vertices, out);
      } else if (*it == "merge") {
        ++it;
        std::string newGraph = *it;
        ++it;
        std::string oldGraph1 = *it;
        ++it;
        std::string oldGraph2 = *it;
        cmdMerge(graphs, newGraph, oldGraph1, oldGraph2, out);
      } else if (*it == "extract") {
        ++it;
        std::string newGraph = *it;
        ++it;
        std::string oldGraph = *it;
        ++it;
        size_t n = stoi(*it);
        List< std::string > vertices;
        for (size_t i = 0; i < n; i++) {
          ++it;
          vertices.insertSorted(*it);
        }
        cmdExtract(graphs, newGraph, oldGraph, n, vertices, out);
      }
    }
  }
}

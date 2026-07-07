#include <iostream>

#include "commands.hpp"
#include "../common/io_utils.hpp"
#include "../common/list.hpp"

namespace hachaturyanov
{
  void readInput(Store &store, std::istream &in)
  {
    std::string line;
    while(std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }
      List< std::string > strs = split(line);
      if (strs.size() > 0) {
        auto it = strs.begin();
        std::string dataset = *it;
        store.push(dataset, Dataset());
        if (strs.size() > 1) {
          do {
            ++it;
            int key = std::stoi(*it);
            ++it;
            std::string value = *it;
            store.get(dataset).push(key, value);
          } while (it != strs.begin());
        }
      } else {
        continue;
      }
    }
  }
}
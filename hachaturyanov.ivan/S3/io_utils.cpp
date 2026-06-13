#include <iostream>
#include "../common/list.hpp"

namespace hachaturyanov
{
  List< std::string > split(std::string line)
  {
    List< std::string > result;
    std::string substr;
    bool has = false;
    for (size_t i = 0; i < line.size(); i++) {
      char c = line[i];
      if (c == ' ' || c == '\t' || c == '\n') {
        if (has) {
          result.addEnd(substr);
          substr.clear();
          has = false;
        }
      } else {
        substr.push_back(c);
        has = true;
      }
    }
    if (has) {
      result.addEnd(substr);
    }
    return result;
  }
}

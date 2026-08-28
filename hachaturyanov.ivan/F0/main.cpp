#include <iostream>
#include "commands.hpp"

int main()
{
  hachaturyanov::MatrixTable matrices;
  std::string line;

  while (std::getline(std::cin, line)) {
    const hachaturyanov::List< std::string > &tokens = hachaturyanov::tokenize(line);
    if (tokens.isEmpty()) {
      continue;
    }

    auto handler = hachaturyanov::findHandler(*tokens.begin());
    if (handler == nullptr) {
      std::cout << "<INVALID COMMAND>" << '\n';
      continue;
    }
    handler(tokens, matrices, std::cout);
  }

  return 0;
}

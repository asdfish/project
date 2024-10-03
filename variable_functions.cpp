#include "variable_functions.hpp"
#include <iostream>

std::string append(std::string header, std::string footer) {
  std::string output = header + footer;
  return output;
}

std::string set(std::string unused, std::string output) {
  return output;
}

std::string prompt(std::string unused, std::string text) {
  std::cout << text;

  std::string output;
  std::cin >> output;

  return output;
}

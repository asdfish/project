#include <variable_functions.hpp>
#include <iostream>
#include <string>

void append(std::string* string_p, std::string footer) {
  string_p->append(footer);
}

void set(std::string* string_p, std::string new_string) {
  *string_p = new_string;
}

void prompt(std::string* string_p, std::string text) {
  std::cout << text;
  std::getline(std::cin, *string_p);
}

#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <array>
#include <string>
#include <vector>

struct VariableDescriptor {
  std::string name;
  std::string argument;
  void (*function) (std::string*, std::string);
};

struct Variable {
  std::string name;
  std::string contents;
};

struct Dependency {
  std::string name;
  std::vector<VariableDescriptor> modified_variables;
};

struct ProjectTemplate {
  std::string name;
  std::vector<std::string> directories;
  std::vector<std::array<std::string, 2>> files;
  std::vector<VariableDescriptor> initial_variables;
  std::vector<Dependency> dependencies;
};

#endif

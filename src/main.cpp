#include <config.hpp>
#include <structs.hpp>
#include <variable_functions.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

void cleanup_selections(std::vector<std::vector<bool*>>* selections);
void set_variable(VariableDescriptor*, std::vector<Variable>*);
std::string process_string(std::string, std::vector<Variable>*);


int main() {
  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::FitComponent();
  
  std::vector<std::string> project_template_names;
  project_template_names.reserve(project_templates.size());
  for(unsigned int i = 0; i < project_templates.size(); i ++)
    project_template_names.push_back(project_templates[i].name);

  int project_template = 0;
  ftxui::Component menu = ftxui::Menu(&project_template_names, &project_template);

  ftxui::Component tabs = ftxui::Container::Tab({}, &project_template);
  std::vector<std::vector<bool*>> selections;
  selections.reserve(project_templates.size());
  for(unsigned int i = 0; i < project_templates.size(); i ++) {
    std::vector<bool*> selections_buffer;
    ftxui::Component tab_buffer = ftxui::Container::Vertical({});

    selections_buffer.reserve(project_templates[i].dependencies.size());
    for(unsigned int j = 0; j < project_templates[i].dependencies.size(); j ++) {
      bool* selected = new bool(false);
      selections_buffer.push_back(selected);

      ftxui::Component checkbox = ftxui::Checkbox(project_templates[i].dependencies[j].name, selected);
      tab_buffer->Add(checkbox);
    }
    selections.push_back(selections_buffer);
    tabs->Add(tab_buffer);
  }

  unsigned int exit_code = 1;
  ftxui::Component buttons = ftxui::Container::Vertical({
    ftxui::Button(" Submit ", [&] {
      screen.Exit();
      exit_code = 0;
    }),
    ftxui::Button(" Quit ", [&] {
      screen.Exit();
    }),
  });

  ftxui::Component container = ftxui::Container::Horizontal({
    menu,
    tabs,
    buttons,
  });

  ftxui::Component renderer = ftxui::Renderer(container, [&] {
    return ftxui::hbox({
      ftxui::vbox({
        ftxui::text(" Template: "),
        ftxui::separator(),
        menu->Render(),
      }) | ftxui::vscroll_indicator,
      ftxui::separator(),
      ftxui::vbox({
        ftxui::text(" Dependencies: "),
        ftxui::separator(),
        tabs->Render(),
      }) | ftxui::vscroll_indicator,
      ftxui::separator(),
      ftxui::vbox({
        ftxui::text(" Options: "),
        ftxui::separator(),
        buttons->Render(),
      }),
    }) | ftxui::border;
  });

  screen.Loop(renderer);

  if(exit_code == 1) {
    cleanup_selections(&selections);
    return 0;
  }

  ProjectTemplate* project_template_pointer = &project_templates[project_template];
  std::vector<bool*>* selections_pointer = &selections[project_template];

  std::vector<Variable> variables;
  variables.reserve(project_template_pointer->initial_variables.size());

  for(unsigned int i = 0; i < project_template_pointer->initial_variables.size(); i ++)
    set_variable(&project_template_pointer->initial_variables[i], &variables);

  for(unsigned int i = 0; i < selections_pointer->size(); i ++)
    if(*selections_pointer->at(i))
      for(unsigned int j = 0; j < project_template_pointer->dependencies[i].modified_variables.size(); j ++)
        set_variable(&project_template_pointer->dependencies[i].modified_variables[j], &variables);

  for(unsigned int i = 0; i < project_template_pointer->directories.size(); i ++) {
    std::string processed_path = process_string(project_template_pointer->directories[i], &variables);

    if(std::filesystem::exists(processed_path))
      std::cout << "Directory " << processed_path << " already exists\n";
    else
      std::filesystem::create_directory(processed_path);
  }

  for(unsigned int i = 0; i < project_template_pointer->files.size(); i ++) {
    std::string processed_path = process_string(project_template_pointer->files[i][0], &variables);

    if(std::filesystem::exists(processed_path)) {
      std::cout << "File " << processed_path << " already exists\n";
      continue;
    }

    std::string processed_contents = process_string(project_template_pointer->files[i][1], &variables);

    std::ofstream file(processed_path);
    if(file.fail())
      std::cout << "An error occured writing to file " << processed_path << '\n';
    else
      file << processed_contents;

    file.close();
  }

  cleanup_selections(&selections);
  return 0;
}

void cleanup_selections(std::vector<std::vector<bool*>>* selections) {
  for(unsigned int i = 0; i < selections->size(); i ++)
    for(unsigned int j = 0; j < selections->at(i).size(); j ++)
      delete selections->at(i)[j];
}

void set_variable(VariableDescriptor* variable_descriptor, std::vector<Variable>* variables) {
  for(unsigned int i = 0; i < variables->size(); i ++) {
    if(variables->at(i).name == variable_descriptor->name) {
      variable_descriptor->function(&variables->at(i).contents, variable_descriptor->argument);
      return;
    }
  }

  Variable variable;
  variable.name = variable_descriptor->name;
  variable.contents = "";
  variable_descriptor->function(&variable.contents, variable_descriptor->argument);

  variables->push_back(variable);
}

std::string process_string(std::string original, std::vector<Variable>* variables) {
  for(unsigned int i = 0; i < variables->size(); i ++) {
    std::string::size_type location;
    while((location = original.find(variables->at(i).name)) != std::string::npos) {
      original.erase(location, variables->at(i).name.length());
      original.insert(location, variables->at(i).contents);
    }
  }

  return original;
}

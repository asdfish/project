#pragma once
static std::vector<ProjectTemplate> project_templates = {
  {
    .name = "c",
    .directories = { "#PROJECT_NAME" },
    .files = {
      {
        .path = "#PROJECT_NAME/main.c",
        .contents = "int main() {\n"
                    "  return 0;\n"
                    "}\n",
      },
      {
        .path = "#PROJECT_NAME/Makefile",
        .contents = "STANDARD := -std=c99\n"
                    "LINK_FLAGS := #LINK_FLAGS\n"
                    "INCLUDE_FLAGS := #INCLUDE_FLAGS\n"
                    "SOURCE_FILES := main.c\n"
                    "HEADER_FILES :=\n"
                    "DEBUG_FLAGS := -Wall -Wextra -Werror -Wpedantic\n"
                    "OPTIMIZATION_FLAGS := -Og\n"
                    "CC ?= gcc\n"
                    "\n"
                    "all: #TARGET_NAMES\n"
                    "\t${CC} ${STANDARD} ${SOURCE_FILES} ${LINK_FLAGS} ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o #PROJECT_NAME\n"
                    "\n"
                    "dependencies_prep:\n"
                    "ifeq (, $(wildcard deps))\n"
                    "\tmkdir deps\n"
                    "endif\n"
                    "\n"
                    "#TARGETS\n",
      },
    },
    .initial_variables = {
      {
        .name = "#PROJECT_NAME",
        .argument = "Project Name: ",
        .function = prompt,
      },
      {
        .name = "#INCLUDE_FLAGS",
        .argument = "",
        .function = set,
      },
      {
        .name = "#LINK_FLAGS",
        .argument = "",
        .function = set,
      },
      {
        .name = "#TARGET_NAMES",
        .argument = "",
        .function = set,
      },
      {
        .name = "#TARGETS",
        .argument = "",
        .function = set,
      },
    },
    .dependencies = {
      {
        .name = "miniaudio",
        .modified_variables = {
          {
            .name = "#INCLUDE_FLAGS",
            .argument = " -Ideps/miniaudio",
            .function = append
          },
          {
            .name = "#LINK_FLAGS",
            .argument = " -ldl -lm -lpthread",
            .function = append,
          },
          {
            .name = "#TARGET_NAMES",
            .argument = " miniaudio",
            .function = append
          },
          {
            .name = "#TARGETS",
            .argument = "\n"
                        "miniaudio: dependencies_prep\n"
                        "ifeq (, $(wildcard deps/miniaudio))\n"
                        "\tgit -C deps clone https://github.com/mackron/miniaudio --depth=1 --branch=0.11.21\n"
                        "endif",
            .function = append,
          },
        },
      },
      {
        .name = "termbox2",
        .modified_variables = {
          {
            .name = "#INCLUDE_FLAGS",
            .argument = " -Ideps/termbox2",
            .function = append
          },
          {
            .name = "#LINK_FLAGS",
            .argument = " -Ldeps/termbox2 -ltermbox",
            .function = append,
          },
          {
            .name = "#TARGET_NAMES",
            .argument = " termbox2",
            .function = append,
          },
          {
            .name = "#TARGETS",
            .argument = "\n"
                        "termbox2: dependencies_prep\n"
                        "ifeq (, $(wildcard deps/termbox2))\n"
                        "\tgit -C deps clone https://github.com/termbox/termbox2 --depth=1 --branch=v2.0.0\n"
                        "\tmake -C deps/termbox2\n"
                        "\trm deps/termbox2/*.o deps/termbox2/*.so\n"
                        "endif\n",
            .function = append,
          },
        },
      },
    },
  },
  {
    .name = "c++",
    .directories = { "#PROJECT_NAME" },
    .files = {
      {
        .path = "#PROJECT_NAME/main.cpp",
        .contents = "int main() {\n"
                    "  return 0;\n"
                    "}\n"
      },
      {
        .path = "#PROJECT_NAME/Makefile",
        .contents = "STANDARD := -std=c++20\n"
                    "LINK_FLAGS := #LINK_FLAGS\n"
                    "SOURCE_FILES := main.cpp\n"
                    "HEADER_FILES :=\n"
                    "DEBUG_FLAGS := -Wall -Wextra -Werror -Wpedantic\n"
                    "OPTIMIZATION_FLAGS := -Og\n"
                    "CXX ?= g++\n"
                    "\n"
                    "all: #TARGET_NAMES\n"
                    "\t${CXX} ${STANDARD} ${SOURCE_FILES} ${LINK_FLAGS} ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o #PROJECT_NAME\n"
                    "\n"
                    "dependencies_prep:\n"
                    "ifeq (, $(wildcard deps))\n"
                    "\tmkdir deps\n"
                    "endif\n"
                    "\n"
                    "#TARGETS\n",
      }
    },
    .initial_variables = {
      {
        .name = "#PROJECT_NAME",
        .argument = "Project Name: ",
        .function = prompt
      },
      {
        .name = "#INCLUDE_FLAGS",
        .argument = "",
        .function = set
      },
      {
        .name = "#LINK_FLAGS",
        .argument = "",
        .function = set
      },
      {
        .name = "#TARGET_NAMES",
        .argument = "",
        .function = set
      },
      {
        .name = "#TARGETS",
        .argument = "",
        .function = set
      },
    },
    .dependencies = {
      {
        .name = "ftxui",
        .modified_variables = {
          {
            .name = "#INCLUDE_FLAGS",
            .argument = " -Ideps/ftxui/include",
            .function = append
          },
          {
            .name = "#LINK_FLAGS",
            .argument = " -lftxui-component -lftxui-dom -lftxui-screen",
            .function = append,
          },
          {
            .name = "#TARGET_NAMES",
            .argument = " ftxui",
            .function = append
          },
          {
            .name = "#TARGETS",
            .argument = "\n"
                        "ftxui: dependencies_prep\n"
                        "ifeq (, $(wildcard deps/ftxui))\n"
                        "\tgit -C deps clone https://github.com/ArthurSonzogni/ftxui --depth=1 --branch=v5.0.0\n"
                        "\tmkdir deps/ftxui/build\n"
                        "\tcmake -S deps/ftxui -B deps/ftxui/build\n"
                        "\tmake -C deps/ftxui/build\n"
                        "endif\n",
            .function = append
          }
        },
      },
    },
  },
};

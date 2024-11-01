#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <structs.hpp>
#include <variable_functions.hpp>

static std::vector<ProjectTemplate> project_templates = {
  {
    .name = "c",
    .directories = { "#PROJECT_NAME", "#PROJECT_NAME/src" },
    .files = {
      {
        "#PROJECT_NAME/src/main.c",
          "int main() {\n"
          "  return 0;\n"
          "}\n",
      },
      {
        "#PROJECT_NAME/Makefile",
          "CC ?= cc\n"
          "C_STANDARD := -std=c99\n"
          "\n"
          "LINK_FLAGS := #LINK_FLAGS\n"
          "INCLUDE_FLAGS := #INCLUDE_FLAGS\n"
          "\n"
          "DEBUG_FLAGS := -Wall -Wextra -Wpedantic\n"
          "OPTIMIZATION_FLAGS := -Og -g\n"
          "\n"
          "INSTALL_DIRECTORY := /usr/local/bin\n"
          "\n"
          "OBJECT_FILES := #OBJECT_FILES\n"
          "\n"
          "define COMPILE\n"
          "\t${CC} -xc -c $(1) ${C_STANDARD} ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o $(2)\n"
          "\n"
          "endef\n"
          "\n"
          "all: deps build #PROJECT_NAME\n"
          "\n"
          "deps:\n"
          "\tmkdir deps\n"
          "\n"
          "build:\n"
          "\tmkdir build\n"
          "\n"
          "#TARGETS"
          "build/main.c.o: src/main.c\n"
          "\t$(call COMPILE,src/main.c)\n"
          "\n"
          "#PROJECT_NAME: ${OBJECT_FILES}\n"
          "\t${CC} ${OBJECT_FILES} ${LINK_FLAGS} -o #PROJECT_NAME\n"
          "\n"
          "clean:\n"
          "ifneq (, $(wildcard build))\n"
          "\trm -rf build\n"
          "endif\n"
          "ifneq (, $(wildcard deps))\n"
          "\trm -rf deps\n"
          "endif\n"
          "ifneq (, $(wildcard #PROJECT_NAME))\n"
          "\trm -f #PROJECT_NAME\n"
          "endif\n"
          "\n"
          "install: all ${INSTALL_DIRECTORY}\n"
          "\tcp -f #PROJECT_NAME ${INSTALL_DIRECTORY}\n"
          "\n"
          "uninstall:\n"
          "ifneq (, $(wildcard ${INSTALL_DIRECTORY}/#PROJECT_NAME))\n"
          "\trm -f ${INSTALL_DIRECTORY}/#PROJECT_NAME\n"
          "endif\n"
      },
    },
    .initial_variables = {
      { "#PROJECT_NAME",  "Project Name: ",  prompt, },
      { "#LINK_FLAGS",    "",                set },
      { "#INCLUDE_FLAGS", "",                set },
      { "#OBJECT_FILES",  "build/main.c.o ", set },
      { "#TARGETS",       "",                set },
    },
    .dependencies = {
      {
        "termbox2",
        {
          { "#INCLUDE_FLAGS", "-Ideps/termbox2", append },
          { "#OBJECT_FILES", "build/termbox2.h.o ", append },
          { "#TARGETS",
            "deps/termbox2:\n"
            "\tgit -C deps clone https://github.com/termbox/termbox2 --depth=1\n"
            "\n"
            "build/termbox2.h.o: deps/termbox2\n"
            "\t$(call COMPILE,deps/termbox2/termbox2.h,-DTB_IMPL -w)\n"
            "\n",
            append,
          },
        },
      },
      {
        "miniaudio",
        {
          { "#INCLUDE_FLAGS", "-Ideps/miniaudio", append },
          { "#LINK_FLAGS", "-ldl -lpthread -lm ", append },
          { "#OBJECT_FILES", "build/miniaudio.h.o ", append },
          { "#TARGETS",
            "deps/miniaudio:\n"
            "\tgit -C deps clone https://github.com/mackron/miniaudio --depth=1\n"
            "\n"
            "build/miniaudio.h.o: deps/miniaudio\n"
            "\t$(call COMPILE,deps/miniaudio/miniaudio.h,-DMA_IMPLEMENTATION -w)\n"
            "\n",
            append
          },
        },
      },
    },
  },
};

#endif

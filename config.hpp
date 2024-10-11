#ifndef CONFIG_HPP
#define CONFIG_HPP

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
          "#PROJECT_NAME_OBJECT_FILES := build/main.c.o\n"
          "\n"
          "define COMPILE\n"
          "\t${CC} ${C_STANDARD} -c $(1) ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o\n"
          "\n"
          "endef\n"
          "\n"
          "all: deps build #TARGET_NAMES #PROJECT_NAME\n"
          "\n"
          "deps:\n"
          "\tmkdir deps\n"
          "\n"
          "build:\n"
          "\tmkdir build\n"
          "\n"
          "#TARGETS\n"
          "\n"
          "build/main.c.o: src/main.c\n"
          "\t$(call COMPILE,src/main.c)\n"
          "\n"
          "#PROJECT_NAME: ${#PROJECT_NAME_OBJECT_FILES}\n"
          "\t${CC} ${#PROJECT_NAME_OBJECT_FILES} ${LINK_FLAGS} -o #PROJECT_NAME\n"
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
      { "#PROJECT_NAME",  "Project Name: ", prompt, },
      { "#LINK_FLAGS",    "",               set },
      { "#INCLUDE_FLAGS", "",               set },
      { "#TARGET_NAMES",  "",               set },
      { "#TARGETS",       "",               set },
    },
    .dependencies = {
      {
        "termbox2",
        {
          { "#TARGET_NAMES", "deps/termbox2 ", append, },
          { "#TARGETS",
            "deps/termbox2:\n"
            "\tgit -C deps clone https://github.com/termbox/termbox2 --depth=1\n"
            "\t$(MAKE) -C deps/termbox2 libtermbox2.a\n"
            "\n",
            append,
          },
          { "#LINK_FLAGS", "-Ldeps/termbox2 -ltermbox2 ", append, },
          { "#INCLUDE_FLAGS", "-Ideps/termbox2 ", append, },
        },
      },
    },
  },
};

#endif

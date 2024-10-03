#pragma once
static Dependency raylib = {
  .name = "raylib",
  .modified_variables = {
    {
      .name = "#INCLUDE_FLAGS",
      .argument = " -Ideps/raylib/build/raylib/include",
      .function = append
    },
    {
      .name = "#LINK_FLAGS",
      .argument = "-Ldeps/raylib/build/raylib -lraylib",
      .function = append,
    },
    {
      .name = "#TARGET_NAMES",
      .argument = " raylib",
      .function = append,
    },
    {
      .name = "#TARGETS",
      .argument = "raylib: dependencies_prep\n"
                  "ifeq (, $(wildcard deps/raylib))\n"
                  "\tgit -C deps clone https://github.com/raysan5/raylib --depth=1 --branch=5.0\n"
                  "\tmkdir deps/raylib/build\n"
                  "\tcmake -S deps/raylib -B deps/raylib/build -DBUILD_EXAMPLES=OFF -DPLATFORM=Desktop\n"
                  "\tmake -C deps/raylib/build\n"
                  "endif\n",
      .function = append
    }
  }
};
static Dependency miniaudio = {
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
};

static Dependency termbox2 = {
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
};

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
          "INSTALL_DIRECTORY := /usr/local/bin\n"
          "\n"
          "OBJECT_FILES :=\n"
          "\n"
          "define COMPILE_FILE\n"
          "\t${CC} -c ${STANDARD} $(1) ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o \n"
          "\t$(eval OBJECT_FILES+=build/$(notdir $(1)).o)\n"
          "\n"
          "endef\n"
          "\n"
          "all: #TARGET_NAMES compile\n"
          "\n"
          "compile: ${SOURCE_FILES} ${HEADER_FILES}\n"
          "ifeq (, $(wildcard build))\n"
          "\tmkdir build\n"
          "endif\n"
          "\t$(foreach SOURCE_FILE,$\\\n"
          "\t  ${SOURCE_FILES},$\\\n"
          "\t  $(call COMPILE_FILE,${SOURCE_FILE})$\\\n"
          "\t)\n"
          "\t${CC} ${OBJECT_FILES} ${LINK_FLAGS} -o #PROJECT_NAME\n"
          "\n"
          "dependencies_prep:\n"
          "ifeq (, $(wildcard deps))\n"
          "\tmkdir deps\n"
          "endif\n"
          "\n"
          "#TARGETS\n"
          "\n"
          "install: ${INSTALL_DIRECTORY}\n"
          "ifeq (, $(wildcard #PROJECT_NAME))\n"
          "\tmake\n"
          "endif\n"
          "\tcp -f #PROJECT_NAME ${INSTALL_DIRECTORY}\n"
          "\n"
          "uninstall:\n"
          "ifneq (, $(wildcard ${INSTALL_DIRECTORY}/#PROJECT_NAME))\n"
          "\trm -f ${INSTALL_DIRECTORY}/#PROJECT_NAME\n"
          "endif\n"
          "\n"
          "clean:\n"
          "ifneq (, $(wildcard #PROJECT_NAME))\n"
          "\trm -f #PROJECT_NAME\n"
          "endif\n"
          "ifneq (, $(wildcard build))\n"
          "\trm -rf build\n"
          "endif\n"
          "ifneq (, $(wildcard deps))\n"
          "\trm -rf deps\n"
          "endif\n"
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
      miniaudio,
      raylib,
      termbox2,
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
          "OBJECT_FILES :=\n"
          "\n"
          "define COMPILE_FILE\n"
          "\t${CXX} ${STANDARD} -c $(1) ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o\n"
          "\t$(eval OBJECT_FILES+=build/$(notdir $(1)).o)\n"
          "\n"
          "endef\n"
          "\n"
          "all: #TARGET_NAMES compile\n"
          "\n"
          "compile: ${SOURCE_FILES} ${HEADER_FILES}\n"
          "ifeq (, $(wildcard build))\n"
          "\tmkdir build\n"
          "endif\n"
          "\t$(foreach SOURCE_FILE,$\\\n"
          "\t\t${SOURCE_FILES},$\\\n"
          "\t\t$(call COMPILE_FILE,${SOURCE_FILE})$\\\n"
          ")\n"
          "\t${CXX} ${OBJECT_FILES} ${LINK_FLAGS} -o #PROJECT_NAME\n"
          "\n"
          "dependencies_prep:\n"
          "ifeq (, $(wildcard deps))\n"
          "\tmkdir deps\n"
          "endif\n"
          "\n"
          "#TARGETS\n"
          "\n"
          "install: ${INSTALL_DIRECTORY}\n"
          "ifeq (, $(wildcard #PROJECT_NAME))\n"
          "\tmake\n"
          "endif\n"
          "\tcp -f #PROJECT_NAME ${INSTALL_DIRECTORY}\n"
          "\n"
          "uninstall:\n"
          "ifneq (, $(wildcard ${INSTALL_DIRECTORY}/#PROJECT_NAME))\n"
          "\trm -f ${INSTALL_DIRECTORY}/#PROJECT_NAME\n"
          "endif\n"
          "\n"
          "clean:\n"
          "ifneq (, $(wildcard #PROJECT_NAME))\n"
          "\trm -f #PROJECT_NAME\n"
          "endif\n"
          "ifneq (, $(wildcard build))\n"
          "\trm -rf build\n"
          "endif\n"
          "ifneq (, $(wildcard deps))\n"
          "\trm -rf deps\n"
          "endif\n"
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
      miniaudio,
      raylib,
      termbox2,
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
            .argument = " -Ldeps/ftxui/build -lftxui-component -lftxui-dom -lftxui-screen",
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

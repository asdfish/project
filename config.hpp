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
      .argument = " -Ideps/miniaudio/extras/miniaudio_split",
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
        "miniaudio: build_prep dependencies_prep\n"
        "ifeq (, $(wildcard deps/miniaudio))\n"
        "\tgit -C deps clone https://github.com/mackron/miniaudio --depth=1 --branch=0.11.21\n"
        "\t${CC} -c deps/miniaudio/extras/miniaudio_split/miniaudio.c -o build/miniaudio.c.o\n"
        "endif\n",
      .function = append,
    },
    {
      .name = "#OBJECT_FILES",
      .argument = " build/miniaudio.c.o",
      .function = append
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
          "STANDARD := -std=c99\n"
          "LINK_FLAGS := #LINK_FLAGS\n"
          "INCLUDE_FLAGS := #INCLUDE_FLAGS\n"
          "SOURCE_FILES := src/main.c\n"
          "HEADER_FILES :=\n"
          "DEBUG_FLAGS := -Wall -Wextra -Wpedantic\n"
          "OPTIMIZATION_FLAGS := -Og\n"
          "CC ?= gcc\n"
          "INSTALL_DIRECTORY := /usr/local/bin\n"
          "\n"
          "OBJECT_FILES := #OBJECT_FILES\n"
          "\n"
          "define COMPILE_FILE\n"
          "\t${CC} -c ${STANDARD} $(1) ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o \n"
          "\t$(eval OBJECT_FILES+=build/$(notdir $(1)).o)\n"
          "\n"
          "endef\n"
          "\n"
          "all: #TARGET_NAMES compile\n"
          "\n"
          "compile: build_prep ${SOURCE_FILES} ${HEADER_FILES}\n"
          "\t$(foreach SOURCE_FILE,$\\\n"
          "\t  ${SOURCE_FILES},$\\\n"
          "\t  $(call COMPILE_FILE,${SOURCE_FILE})$\\\n"
          "\t)\n"
          "\t${CC} ${OBJECT_FILES} ${LINK_FLAGS} -o #PROJECT_NAME\n"
          "\n"
          "build_prep:\n"
          "ifeq (, $(wildcard build))\n"
          "\tmkdir build\n"
          "endif\n"
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
      { "#PROJECT_NAME",  "Project Name: ", prompt, },
      { "#SOURCE_FILES",  "",               set, },
      { "#INCLUDE_FLAGS", "",               set, },
      { "#LINK_FLAGS",    "",               set, },
      { "#OBJECT_FILES",  "",               set },
      { "#TARGET_NAMES",  "",               set },
      { "#TARGETS",       "",               set, },
    },
    .dependencies = {
      miniaudio,
      raylib,
      termbox2,
    },
  },
};

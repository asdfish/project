CXX ?= g++

STANDARD := -std=c++20

LINK_FLAGS := -Ldeps/ftxui/build -lftxui-component -lftxui-dom -lftxui-screen
INCLUDE_FLAGS := -Ideps/ftxui/include -Iinclude

DEBUG_FLAGS := -Wall -Wextra -Wpedantic
OPTIMIZATION_FLAGS := -O2 -march=native -pipe

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES := build/main.cpp.o build/variable_functions.cpp.o

define COMPILE_FILE
	${CXX} -c ${STANDARD} $(1) ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o

endef

all: project

build:
	mkdir build

build/main.cpp.o: config.hpp include/variable_functions.hpp src/main.cpp
	$(call COMPILE_FILE,src/main.cpp)

build/variable_functions.cpp.o: include/variable_functions.hpp src/variable_functions.cpp
	$(call COMPILE_FILE,src/variable_functions.cpp)

deps:
	mkdir deps

deps/ftxui: deps
	git -C deps clone https://github.com/ArthurSonzogni/ftxui --depth=1 --branch=v5.0.0
	mkdir deps/ftxui/build
	cmake -S deps/ftxui -B deps/ftxui/build -DFTXUI_QUIET=ON -DFTXUI_ENABLE_INSTALL=OFF
	make -C deps/ftxui/build

project: deps/ftxui build ${OBJECT_FILES}
	${CXX} ${OBJECT_FILES} ${LINK_FLAGS} -o project

clean:
ifneq (, $(wildcard deps))
	rm -rf deps
endif
ifneq (, $(wildcard build))
	rm -rf build
endif
ifneq (, $(wildcard project))
	rm -f project
endif

install: ${INSTALL_DIRECTORY}
ifeq (, $(wildcard project))
	make
endif
	cp -f project ${INSTALL_DIRECTORY}

uninstall:
ifneq (, $(wildcard ${INSTALL_DIRECTORY}/project))
	rm -f ${INSTALL_DIRECTORY}/project
endif

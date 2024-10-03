CXX ?= g++

STANDARD := -std=c++20

SOURCE_FILES := main.cpp variable_functions.cpp
HEADER_FILES := config.hpp variable_functions.hpp

LINK_FLAGS := -Ldeps/ftxui/build -lftxui-component -lftxui-dom -lftxui-screen
INCLUDE_FLAGS := -Ideps/ftxui/include

DEBUG_FLAGS := -Wall -Wextra -Wpedantic
OPTIMIZATION_FLAGS := -O2 -march=native

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES :=

define COMPILE
	${CXX} -c ${STANDARD} $(1) ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o
	$(eval OBJECT_FILES+=build/$(notdir $(1)).o)

endef

all: compile

compile: dependencies ${SOURCE_FILES} ${HEADER_FILES}
ifeq (, $(wildcard build))
	mkdir build
endif
	$(foreach SOURCE_FILE,$\
		${SOURCE_FILES},$\
		$(call COMPILE,${SOURCE_FILE})$\
	)
	${CXX} ${OBJECT_FILES} ${LINK_FLAGS} -o project

dependencies:
ifeq (, $(wildcard deps))
	mkdir deps
endif
ifeq (, $(wildcard deps/ftxui))
	git -C deps clone https://github.com/ArthurSonzogni/ftxui --depth=1 --branch=v5.0.0
	mkdir deps/ftxui/build
	cmake -S deps/ftxui -B deps/ftxui/build -DFTXUI_QUIET=ON -DFTXUI_ENABLE_INSTALL=OFF
	make -C deps/ftxui/build
endif

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

CXX ?= g++
CXX_FLAGS := -std=c++20 $\
						 -O2 -march=native -pipe $\
						 -Wall -Wextra -Wpedantic $\
						 -Ideps/ftxui/include -I. -Iinclude

LD_FLAGS := -Ldeps/ftxui/build -lftxui-component -lftxui-dom -lftxui-screen

INSTALL_DIRECTORY := /usr/local/bin

# comment/uncomment to enable/disable
# PROCESS_HEADER_FILES := yes
PROCESSED_HEADER_FILES := $(if ${PROCESS_HEADER_FILES},$\
														$(subst .hpp,$\
															$(if $(findstring clang++,${CXX}),$\
																.hpp.pch,$\
																.hpp.gch),$\
															$(shell find -name '*.hpp' -not -path './deps/*')))
OBJECT_FILES := $(patsubst src/%.cpp,$\
									build/%.o,$\
									$(shell find src -name '*.cpp'))

FTXUI_LIBS := deps/ftxui/build/libftxui-component.a deps/ftxui/build/libftxui-dom.a deps/ftxui/build/libftxui-screen.a

PROJECT_REQUIREMENTS := ${FTXUI_LIBS} ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}

define REMOVE
$(if $(wildcard $(1)),$\
	rm $(1))

endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(call REMOVE,${ITEM}))

endef

all: project

%.gch: %
	${CXX} -c $< ${CXX_FLAGS}

%.pch: %
	${CXX} -c $< ${CXX_FLAGS}

build/%.o :src/%.cpp
	${CXX} -c $< ${CXX_FLAGS} -o $@

deps/ftxui/build:
	mkdir deps/ftxui/build

deps/ftxui/build/Makefile: deps/ftxui/build
	cmake -S deps/ftxui -B deps/ftxui/build -DFTXUI_QUIET=ON -DFTXUI_ENABLE_INSTALL=OFF

${FTXUI_LIBS}: deps/ftxui/build/Makefile
	make -C deps/ftxui/build

project: ${PROJECT_REQUIREMENTS}
	${CXX} ${OBJECT_FILES} ${LD_FLAGS} -o project

clean:
	$(call REMOVE_LIST,${PROJECT_REQUIREMENTS})

install: all ${INSTALL_DIRECTORY} uninstall
	cp project ${INSTALL_DIRECTORY}

uninstall:
	$(call REMOVE,${INSTALL_DIRECTORY}/project)

.PHONY: all clean install uninstall

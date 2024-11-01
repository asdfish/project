CXX ?= g++
CXX_FLAGS := -std=c++20 $\
						 -O2 -march=native -pipe $\
						 -Wall -Wextra -Wpedantic $\
						 -Ideps/ftxui/include -Iinclude

LD_FLAGS := -Ldeps/ftxui/build -lftxui-component -lftxui-dom -lftxui-screen

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES := $(patsubst src/%.cpp,$\
									build/%.o,$\
									$(shell find src -name '*.cpp'))

FTXUI_LIBS := deps/ftxui/build/libftxui-component.a deps/ftxui/build/libftxui-dom.a deps/ftxui/build/libftxui-screen.a

define REMOVE_LIST
	$(foreach ITEM,$\
		$(1),$\
		$(if $(wildcard ${ITEM}),$\
			$(shell rm ${ITEM})))

endef

all: ${FTXUI_LIBS} project

build/%.o :src/%.cpp
	${CXX} -c $< ${CXX_FLAGS} -o $@

${FTXUI_LIBS}:
	mkdir deps/ftxui/build
	cmake -S deps/ftxui -B deps/ftxui/build -DFTXUI_QUIET=ON -DFTXUI_ENABLE_INSTALL=OFF
	make -C deps/ftxui/build

project: ${OBJECT_FILES}
	${CXX} ${OBJECT_FILES} ${LD_FLAGS} -o project

clean:
	$(call REMOVE_LIST,$\
		${OBJECT_FILES})
ifneq (,$(wildcard project))
	rm project
endif

install: all ${INSTALL_DIRECTORY} uninstall
	cp project ${INSTALL_DIRECTORY}

uninstall:
ifneq (,$(wildcard ${INSTALL_DIRECTORY}/project))
	rm ${INSTALL_DIRECTORY}/project
endif

.PHONY: all clean install uninstall

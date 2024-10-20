CXX ?= g++
CXX_FLAGS := -std=c++20 $\
						 -O2 -march=native -pipe $\
						 -Wall -Wextra -Wpedantic $\
						 -Ideps/ftxui/include -Iinclude $\

LD_FLAGS := -Ldeps/ftxui/build -lftxui-component -lftxui-dom -lftxui-screen

DIRECTORIES := build deps
DEPENDENCIES := deps/ftxui

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES := build/main.o build/variable_functions.o

all: ${DIRECTORIES} ${DEPENDENCIES} project

${DIRECTORIES}:
	$(foreach DIRECTORY,$\
		${DIRECTORIES},$\
		$(if $(wildcard ${DIRECTORY}),,$\
			$(shell mkdir ${DIRECTORY})$\
		)$\
	)

${OBJECT_FILES}: build/%.o :src/%.cpp
	${CXX} -c $< ${CXX_FLAGS} -o $@

deps/ftxui:
	git -C deps clone https://github.com/ArthurSonzogni/ftxui --depth=1 --branch=v5.0.0
	mkdir deps/ftxui/build
	cmake -S deps/ftxui -B deps/ftxui/build -DFTXUI_QUIET=ON -DFTXUI_ENABLE_INSTALL=OFF
	make -C deps/ftxui/build

project: ${OBJECT_FILES}
	${CXX} ${OBJECT_FILES} ${LD_FLAGS} -o project

clean:
	-rm -rf deps
	-rm -rf build
	-rm -f project

install: all ${INSTALL_DIRECTORY}
	cp -f project ${INSTALL_DIRECTORY}

uninstall:
ifneq (, $(wildcard ${INSTALL_DIRECTORY}/project))
	rm -f ${INSTALL_DIRECTORY}/project
endif

.PHONY: all clean

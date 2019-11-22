SRC_FOLDER = src/
SRCS = $(wildcard ${SRC_FOLDER}*.cpp)

WARNINGS = -Wno-writable-strings

build:
	@g++ -std=c++14 ${WARNINGS} ${SRCS} main.cpp -o  bin/TextIndexer

.PHONY : build

SRC_FOLDER = src/
BUILD_FOLDER = build/

SRCS = $(wildcard ${SRC_FOLDER}*.cpp)

WARNINGS = -Wno-writable-strings
EXE = TextIndexer

build:
	@g++ -std=c++14 ${WARNINGS} ${SRCS} main.cpp -o ${EXE}
	@mv ${EXE} ${BUILD_FOLDER}${EXE}

.PHONY : build

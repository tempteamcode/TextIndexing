SRC_FOLDER = src/
BUILD_FOLDER = build/

SRCS = $(wildcard ${SRC_FOLDER}*.cpp)

WARNINGS = -Wall -Wpedantic -Wno-writable-strings -Wno-unused-variable
FLAGS = -O3
EXE = TextIndexer

exec:
	@echo "\n\n=== Executing the text indexer ===\n\n"
	@-./${BUILD_FOLDER}${EXE}
	@echo "\n\n"


build:
	g++ -std=c++14 ${WARNINGS} ${FLAGS} ${SRCS} main.cpp -o ${EXE}
	@mkdir -p ./build
	@mv ${EXE} ${BUILD_FOLDER}${EXE}

.PHONY : build

clean:
	@rm -rf ./build

.PHONY: clean

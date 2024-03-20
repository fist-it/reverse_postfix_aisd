all: read

build:
	cmake --build build

run: build
	@./build/build/executable/executable > out.txt

read: run
	@cat out.txt

debug:
	lldb ./build/build/executable/executable

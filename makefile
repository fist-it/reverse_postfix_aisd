all: compile run

compile:
	g++ -g *.cpp -o ./build/executable

run:
	./build/executable > out.txt

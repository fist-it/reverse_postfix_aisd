all: compile run

compile:
	g++ -g *.cpp -o executable

run:
	./build/executable

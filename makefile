all: compile run show_out

compile:
	@g++ -g *.cpp -o ./build/executable && echo "compiled, input:"

run:
	@./build/executable > out.txt

show_out:
	@cat out.txt

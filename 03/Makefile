CC = g++
FLAGS = -Wall -Werror -Wno-unused-comparison -Wno-self-assign-overloaded -std=c++17 -fsanitize=address

all: matrix

test: matrix
	./$<

matrix: tests.o matrix.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp Matrix.hpp
	${CC} ${FLAGS} -c -o $@ $<

matrix.o: Matrix.cpp Matrix.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o matrix
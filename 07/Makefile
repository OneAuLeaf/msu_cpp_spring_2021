CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: vector

test: vector
	./$<

vector: tests.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp Vector.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o vector
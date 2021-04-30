CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: format

test: format
	./$<

format: tests.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp Format.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o format
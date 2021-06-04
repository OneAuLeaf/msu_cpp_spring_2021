CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address -pthread

all: externsort

test: externsort
	./$<

externsort: tests.o externsort.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp ExternSort.hpp
	${CC} ${FLAGS} -c -o $@ $<

externsort.o: ExternSort.cpp ExternSort.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o externsort
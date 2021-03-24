CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: allocator

test:
	./allocator

allocator: tests.o allocator.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp Allocator.hpp
	${CC} ${FLAGS} -c -o $@ $<

allocator.o: Allocator.cpp Allocator.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o allocator
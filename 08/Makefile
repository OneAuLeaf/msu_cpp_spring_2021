CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: threadpool

test: threadpool
	./$<

threadpool: tests.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp ThreadPool.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o threadpool
CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: bigint

test: bigint
	./$<

bigint: tests.o bigint.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp BigInt.hpp
	${CC} ${FLAGS} -c -o $@ $<

bigint.o: BigInt.cpp BigInt.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o bigint
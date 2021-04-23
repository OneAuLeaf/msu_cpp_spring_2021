CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: serializer

test: serializer
	./$<

serializer: tests.o serializer.o
	${CC} ${FLAGS} -o $@ $^

tests.o: tests.cpp Serializer.hpp
	${CC} ${FLAGS} -c -o $@ $<

serializer.o: Serializer.cpp Serializer.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o serializer
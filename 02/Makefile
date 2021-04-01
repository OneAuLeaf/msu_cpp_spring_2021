CC = g++
FLAGS = -Wall -Werror -std=c++17 -fsanitize=address

all: parser

test: parser
	./$<
	
parser: tests.o libparser.a
	${CC} ${FLAGS} -o $@ $< -L. -lparser

tests.o: tests.cpp
	${CC} ${FLAGS} -c -o $@ $<

libparser.a: parser.o
	ar rc $@ $^

parser.o: TokenParser.cpp  TokenParser.hpp
	${CC} ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o *.a parser
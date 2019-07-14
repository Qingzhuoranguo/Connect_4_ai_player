all: test

%.o : %.c Connect4.h
	g++ -c -Wall -Werror -g -std=c++98

test:  Connect4.o Connect4_test.o 
	g++ Connect4.o Connect4_test.o -o test -Wall -Werror -g -std=c++98

clean:
	rm -f core *.o test


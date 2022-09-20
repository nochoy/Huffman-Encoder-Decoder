CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra


all: encode decode

encode: encode.o code.o node.o stack.o pq.o io.o huffman.o
	$(CC) -o encode encode.o code.o node.o stack.o pq.o io.o huffman.o

decode: decode.o code.o node.o stack.o pq.o io.o huffman.o 
	$(CC) -o decode decode.o code.o node.o stack.o pq.o io.o huffman.o

encode.o: encode.c code.c code.h node.c node.h stack.c stack.h pq.c pq.h io.c io.h huffman.c huffman.h
	$(CC) $(CFLAGS) -c encode.c code.c node.c stack.c pq.c io.c huffman.c

decode.o: decode.c code.c code.h node.c node.h stack.c stack.h pq.c pq.h io.c io.h huffman.c huffman.h
	$(CC) $(CFLAGS) -c decode.c code.c node.c stack.c pq.c io.c huffman.c

clean:
	rm -f encode encode.o decode decode.o code.o node.o stack.o pq.o io.o huffman.o

format:
	clang-format -i -style=file *.[ch]



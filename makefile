CC = gcc
PARAMS = -Wall -g

all: clean run remove_o

aes:
	${CC} ${PARAMS} -c aes.c

run: aes
	${CC} ${PARAMS} -o aes aes.o main.c

remove_o:
	rm *.o

clean: 
	rm -f aes

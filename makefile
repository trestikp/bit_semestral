CC = gcc
PARAMS = -Wall -g

all: clean run remove_o

aes:
	${CC} ${PARAMS} -c aes.c

file_loader:
	${CC} ${PARAMS} -c file_loader.c

run: aes file_loader
	${CC} ${PARAMS} -o aes aes.o file_loader.o main.c

remove_o:
	rm *.o

clean: 
	rm -f aes

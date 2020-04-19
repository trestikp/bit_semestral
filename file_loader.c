#include <stdio.h>
#include <stdlib.h>
#include "aes.h"


char dec_to_hex(int n) {
	if(n >= 0 || n < 10) {
		return 48 + n;
	} else if(n >= 10 || n < 16) {
		return 97;
	} else {
		return -1;
	}
}

void to_hex(int c, char *res) {
/* doesn't work, because chars loaded are outside range 0 - 127 
	sprintf(res, "%x", c);	
*/
	// ASSUMING RANGE 0 - 256
	int first = c / 16;
	int rest = c % 16;

	printf("BEFORE: %d %d\n", first, rest);
	printf("AFTER: %c %c\n", dec_to_hex(first), dec_to_hex(rest));

	res[0] = dec_to_hex(first);
	res[1] = dec_to_hex(rest);
}

int read_input(char *file_name, short input[MAX_INPUT_SIZE]) {
	short c = '\0';
	int i = 0;
	FILE *f = fopen(file_name, "rb");

	if(!f) return 0;

	while(c != EOF) {
//	while(1) {
//		if(feof(f)) break;
		c  = fgetc(f);
		input[i] = c;
		//printf("%c = %d\n", c, c);
		//to_hex(c, input[i]);
		//printf("%c", c);
		//input[i] = c;
		i++;
	}

	fclose(f);	
	return 1;
}


void print_input(short input[MAX_INPUT_SIZE]) {
//	read_input("res/message.txt");
	//read_input("res/pokus.txt");
	int i = 0;
	for (i = 0; i < 352; i++) {
		printf("%c = %d\n", input[i], input[i]);
	}
//	printf("%s\n", input);
}

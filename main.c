#include <stdio.h>
#include <string.h>
#include "aes.h"

/**
	Prints help on how to use the program into console.
*/
void print_help() {
	printf("Usage:\n");
	printf("aes <filename> [cipher_key] [output_file]\n");
	printf("\tParameter <filename> is the input file that is to be enrypted.\n This parameter is required.\n");
	printf("\tParameter [cipher_key] custom 16 letter encryption key.\n This parameter is not required.\n");
	printf("\tParameter [output_file] is file to which encrypted message is stored.\n This parameter is not required.\n");
}

/**
	Writes the encryption result to a file in hexadecimal.

	@param file_name = the name of the target file
	@param output = ouput buffer with the encrypted input
	@param output_size = size of the encrypted message (1:1 input length)
*/
int write_output_to_file(char *file_name, u_char *output, int output_size) {
	int i = 0;
	FILE *f = NULL;

	f = fopen(file_name, "w");

	if(!f) return 1;

	for(i = 0; i < output_size; i++) {
		fputc(output[i], f);
	}

	fclose(f);
	return 0;
}

/**
	Reads the input from a file and encrypts it. Reads by 16 bytes and encrypts them
	right away before loading another 16 bytes. If the final state isn't 16 bytes
	long fills with 0 from right.

	@param file_name = input file
	@param size = size of input loaded
*/
int read_input(char *file_name, int *size, u_char key[MAGICAL_SIXTEEN]) {
        short c = '\0';
        int i = 0, j = 0;
	u_char state[MAGICAL_FOUR][MAGICAL_FOUR], round_key[MAGICAL_SIXTEEN * ROUND_COUNT];
	FILE *f;

	//u_char key[MAGICAL_SIXTEEN] = "josefvencasladek";
	key_expansion(key, round_key);
        f = fopen(file_name, "rb");

        if(!f) {
		printf("Failed to open file! Maybe the file doesn't exist\n Use aes -h for help.\n");
		return 0;
	}

        while((c = fgetc(f)) != EOF) {
		// tried if(!((i + 1) % 16)) but i dont know why that doesnt work
		if(!(i % 16) && i > 1) {
			encrypt(state, round_key);
		}
		state[(i % MAGICAL_SIXTEEN) / MAGICAL_FOUR][(i % MAGICAL_SIXTEEN) % MAGICAL_FOUR] = c;
                i++;
        }

	
	// fill state with 0 if input isnt divisible by 16
	if(i % MAGICAL_SIXTEEN) {
		int a = i;
		for(j = a; j < (a + (MAGICAL_SIXTEEN - (a % MAGICAL_SIXTEEN))); j++) {
			state[j % MAGICAL_SIXTEEN / MAGICAL_FOUR][j % MAGICAL_FOUR % MAGICAL_FOUR] = '\0';
			i++;
		}
	}
	// final encrypt
	encrypt(state, round_key);
	
        *size = i;

        fclose(f);      
        return 1;
}

/**
	Processes arguments and calls all the important functions.
*/
void run(int argc, char *argv[]) {
	int file_size = 0;
	u_char key[MAGICAL_SIXTEEN] = "josefvencasladek";

	if(argc < 2) {
		printf("A parameter is required!\n");
		print_help();
		return;
	}

	if(argc > 4) {
		printf("Too many arguments!\n");
		print_help();
		return;
	}

	if(argc == 2 && !strcmp(argv[1], "-h")) {
		print_help();
		return;
	}

	if(argc >= 3) {
		if(strlen(argv[2]) < 16) {
			printf("Entered key is too short!\n");
			return;
		} else if(strlen(argv[2]) > 16) {
			printf("Entered key is too long!\n");
			return;
		} else {
			strcpy((char*) key, argv[2]);
		}
	}
		
	if(read_input(argv[1], &file_size, key))
		if (argc == 4) {
			if(write_output_to_file(argv[3], get_output(), file_size)) {
				printf("Failed to open output file!\n");
				return;
			}
		} else {
			print_output(file_size);
		}
	else {
		printf("Failed to read input file!\n");
		return;
	}
	
	
}

/**
	This doesn't need comment right?
*/
int main(int argc, char *argv[]) {
	run(argc, argv);

	return 0;
}

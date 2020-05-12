#include <stdio.h>
#include <string.h>
#include "aes.h"

/**
	Prints help on how to use the program into console.
*/
void print_help() {
	printf("Usage:\n");
	printf("aes <input_name> -k [cipher_key] -o [output_file]\n\n");
	printf("Parameter <input_name> is the input file that is to be enrypted.\n\tThis parameter is required.\n");
	printf("If option -k is used it is expected to be followed by [cipher_key].\
		\n\tWhich is 16 custom letter encryption key.\n");
	printf("If option -o is used it is expected to be followed by [output_file].\
		\n\tThis is target file where the encrypted data will be written.\n");
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
	Added support function for parametr extraction. Extracts key from argument.
*/
void extract_key(char *argv, u_char key[MAGICAL_SIXTEEN]) {
	if(strlen(argv) < 16) {
		printf("Entered key is too short!\n");
		return;
	} else if(strlen(argv) > 16) {
		printf("Entered key is too long!\n");
		return;
	} else {
		strcpy((char*) key, argv);
	}
}

/**
	Processes arguments and calls all the important functions.
*/
void run(int argc, char *argv[]) {
	int file_size = 0;
	u_char key[MAGICAL_SIXTEEN] = "josefvencasladek";
	char *output_file = NULL;

	if(argc < 2) {
		printf("A parameter is required!\n");
		print_help();
		return;
	}

	if(argc > 6) {
		printf("Too many arguments!\n");
		print_help();
		return;
	}

	switch(argc) {
		case 2: break;
		case 4:
			if(!strcmp(argv[2], "-k")) {
				extract_key(argv[3], key);
			} else if(!strcmp(argv[2], "-o")) {
				output_file = argv[3];
			} else {
				printf("Unknown option %s\n", argv[2]);
				return;
			}
			break;
		case 6:
			if(!strcmp(argv[2], "-k")) {
				extract_key(argv[3], key);
			} else if(!strcmp(argv[2], "-o")) {
				output_file = argv[3];
			} else {
				printf("Unknown option %s\n", argv[2]);
				return;
			}

			if(!strcmp(argv[4], "-k")) {
				extract_key(argv[5], key);
			} else if(!strcmp(argv[4], "-o")) {
				output_file = argv[5];
			} else {
				printf("Unknown option %s\n", argv[4]);
				return;
			}
			break;
		default:
			printf("Invalid argument count!\n");
			print_help();
			return;
	}

	if(!read_input(argv[1], &file_size, key)) {
		printf("Failed to read input file!\n");
		return;
	} else {
		if(output_file) {
			if(write_output_to_file(output_file, get_output(), file_size)) {
				printf("Failed to open output file!\n");
				return;
			}
		} else {
			print_output(file_size);
		}
	}
}

/**
	This doesn't need comment right?
*/
int main(int argc, char *argv[]) {
	run(argc, argv);

	return 0;
}

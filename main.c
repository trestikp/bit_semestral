#include <stdio.h>
#include "aes.h"


void print_help() {
	printf("Usage:\n");
	printf("aes <filename> [output_file]\n");
	printf("\tParameter <filename> is the input file that is to be enrypted.\n This parameter is required.\n");
	printf("\tParameter [output_file] is file to which encrypted message is stored.\n This parameter is not required.\n");
}

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

int read_input(char *file_name, int *size) {
        short c = '\0';
        int i = 0, j = 0;
	u_char state[MAGICAL_FOUR][MAGICAL_FOUR], round_key[MAGICAL_SIXTEEN * ROUND_COUNT];
	FILE *f;

	u_char key[MAGICAL_SIXTEEN] = "josefvencasladek";
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
			append_state_to_output(state, i - MAGICAL_SIXTEEN);
		}
		state[(i % MAGICAL_SIXTEEN) / MAGICAL_FOUR][(i % MAGICAL_SIXTEEN) % MAGICAL_FOUR] = c;
                i++;
        }

	
	if(i % MAGICAL_SIXTEEN) {
		for(j = (i + 1); j < (i + (i % MAGICAL_SIXTEEN)); j++) {
			state[j % MAGICAL_SIXTEEN / MAGICAL_FOUR][j % MAGICAL_FOUR % MAGICAL_FOUR] = '\0';
			i++;
		}
		//encrypt(state, round_key);
		//append_state_to_output(state, i - (MAGICAL_SIXTEEN  - (i % MAGICAL_SIXTEEN)));
	}
	encrypt(state, round_key);
	append_state_to_output(state, i - (MAGICAL_SIXTEEN  - (i % MAGICAL_SIXTEEN)));
	
        *size = i;

        fclose(f);      
        return 1;
}

void run(int argc, char *argv[]) {
	int file_size = 0;

	if(argc < 2) {
		printf("A parameter is required!\n");
		print_help();
		return;
	}

	if(argc > 3) {
		printf("Too many arguments!\n");
		print_help();
		return;
	}

	if(read_input(argv[1], &file_size))
		if(argc == 3) {
			if(write_output_to_file(argv[2], get_output(), file_size)) {
				printf("Failed to open output file!");
			}
		} else {
			print_output(file_size);
		}
	else
		return;
	
	
}

int main(int argc, char *argv[]) {
	run(argc, argv);

	return 0;
	//read_input("res/message.txt", &file_size);
	//read_input("res/Shea.jpg", &file_size);

	//printf("i: %d, i16: %d\n", file_size, file_size%16);

	//print_output(file_size);
}

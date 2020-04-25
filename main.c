#include <stdio.h>
#include "aes.h"

void print_state(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT]) {
        short i = 0, j = 0;
        for(i = 0; i < BLOCK_RC_COUNT; i++) {
                for(j = 0; j < BLOCK_RC_COUNT; j++) {
                        printf("%.2x ", state[i][j]);
                }
                printf("\n");
        }
	printf("----\n");
}

int read_input(char *file_name, int *size) {
        short c = '\0', a, b;
        int i = 0, j = 0;
	unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT], round_key[BLOCK_SIZE * ROUND_COUNT];
	FILE *f;

	unsigned char key[BLOCK_SIZE] = "josefvencasladek";
//	unsigned char key[BLOCK_SIZE] = "Thats my Kung Fu";
	key_expansion(key, round_key);
        f = fopen(file_name, "rb");


	/* key in hex
	for(i = 0; i < 16; i++) {
		printf("%.2x ", key[i]);
	}
	printf("\n");
	*/

        if(!f) return 0;

        /*
        fseek(f, 0L, SEEK_END);
        *size = ftell(f);
        fseek(f, 0L, SEEK_SET);
        */

        while((c = fgetc(f)) != EOF) {
		if(!(i % 16) && i > 1) {
			//print_state(state);
			encrypt(state, round_key);
			append_state_to_output(state, i - BLOCK_SIZE);
			//print_state(state);
		}
		a = (i % BLOCK_SIZE) / BLOCK_RC_COUNT;
		b = (i % BLOCK_SIZE) % BLOCK_RC_COUNT;
		//state[(i % BLOCK_SIZE) / BLOCK_RC_COUNT][(i % BLOCK_SIZE) % BLOCK_RC_COUNT] = c;
		//printf("%d putting %.2x @ %d:%d\n", i, c, a, b);
		state[a][b] = c;
                //input[i] = c;
                i++;

		/*
		if(!((i + 1) % 16)) {
			//print_state(state);
			encrypt(state, round_key);
			append_state_to_output(state, i - BLOCK_SIZE);
			//print_state(state);
		}
                i++;
		*/
        }

	
	if(i % BLOCK_SIZE) {
		for(j = (i % BLOCK_SIZE + 1); j < BLOCK_SIZE; j++) {
			state[j % BLOCK_SIZE / BLOCK_RC_COUNT][j % BLOCK_RC_COUNT % BLOCK_RC_COUNT] = '\0';
		}
		encrypt(state, round_key);
		append_state_to_output(state, i - (BLOCK_SIZE  - (i % BLOCK_SIZE)));
	}
	
        *size = i;

        fclose(f);      
        return 1;
}

int main(int argc, char *argv[]) {
	//unsigned char key[BLOCK_SIZE] = "josefvencasladek";	
	//unsigned char round_key[BLOCK_SIZE * ROUND_COUNT] = {'\0'};

	int file_size = 0;
	//read_input("res/message.txt", &file_size);
	read_input("res/Shea.jpg", &file_size);
//	read_input("res/test.txt", &file_size);
	print_output(file_size);
}

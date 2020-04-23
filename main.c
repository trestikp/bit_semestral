#include <stdio.h>
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

//	unsigned char key[BLOCK_SIZE] = "josefvencasladek";
	unsigned char key[BLOCK_SIZE] = "Thats my Kung Fu";
	key_expansion(key, round_key);
        f = fopen(file_name, "rb");

        if(!f) return 0;

        /*
        fseek(f, 0L, SEEK_END);
        *size = ftell(f);
        fseek(f, 0L, SEEK_SET);
        */

        while((c = fgetc(f)) != EOF) {
		if(!(i % 16) && i > 1) {
		//	print_state(state);
			encrypt(state, round_key);
			print_state(state);
		}
		a = (i % BLOCK_SIZE) / BLOCK_RC_COUNT;
		b = (i % BLOCK_SIZE) % BLOCK_RC_COUNT;
		//state[(i % BLOCK_SIZE) / BLOCK_RC_COUNT][(i % BLOCK_SIZE) % BLOCK_RC_COUNT] = c;
		//printf("%d putting %.2x @ %d:%d\n", i, c, a, b);
		state[a][b] = c;
                //input[i] = c;
                i++;
        }


	
	/*
	if(i % BLOCK_SIZE) {
		for(j = (i % BLOCK_SIZE + 1); j < BLOCK_SIZE; j++) {
			state[j % BLOCK_SIZE / BLOCK_RC_COUNT][j % BLOCK_RC_COUNT % BLOCK_RC_COUNT] = '\0';
		}
		print_state(state);
	} else {
		print_state(state);
	}
	*/
	

	//printf("i: %d\n", i);

        //printf("I: %d", i);
        *size = i;

        fclose(f);      
        return 1;
}

/*
void print_input(short input[MAX_INPUT_SIZE]) {
//      read_input("res/message.txt");
        //read_input("res/pokus.txt");
        int i = 0;
        for (i = 0; i < 352; i++) {
                printf("%c = %d\n", input[i], input[i]);
        }
//      printf("%s\n", input);
}
*/


int main(int argc, char *argv[]) {
	unsigned char key[BLOCK_SIZE] = "josefvencasladek";	
	unsigned char round_key[BLOCK_SIZE * ROUND_COUNT] = {'\0'};

	int file_size = 0;
	read_input("res/message.txt", &file_size);
//	print_output(file_size);
}

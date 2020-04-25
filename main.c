#include <stdio.h>
#include "aes.h"


int read_input(char *file_name, int *size) {
        short c = '\0';
        int i = 0, j = 0;
	u_char state[MAGICAL_FOUR][MAGICAL_FOUR], round_key[MAGICAL_SIXTEEN * ROUND_COUNT];
	FILE *f;

	u_char key[MAGICAL_SIXTEEN] = "josefvencasladek";
	key_expansion(key, round_key);
        f = fopen(file_name, "rb");

        if(!f) return 0;

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
		for(j = (i % MAGICAL_SIXTEEN + 1); j < MAGICAL_SIXTEEN; j++) {
			state[j % MAGICAL_SIXTEEN / MAGICAL_FOUR][j % MAGICAL_FOUR % MAGICAL_FOUR] = '\0';
		}
		encrypt(state, round_key);
		append_state_to_output(state, i - (MAGICAL_SIXTEEN  - (i % MAGICAL_SIXTEEN)));
	}
	
        *size = i;

        fclose(f);      
        return 1;
}

int main(int argc, char *argv[]) {
	int file_size = 0;

	//read_input("res/message.txt", &file_size);
	read_input("res/Shea.jpg", &file_size);

	print_output(file_size);
}

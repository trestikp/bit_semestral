#include <stdio.h>
#include "aes.h"

unsigned char output[MAX_INPUT_SIZE] = {'\0'};
int file_size = 0;

/* "borrowed" from wiki */
//const unsigned char Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
const unsigned char Rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
 

/*
	also "borrowed" from the interent 
	look-up table for byte substitution
*/
static unsigned char s_box[256] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // a
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // b
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // c
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // d
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // e
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // f
	};

void add_round_key(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT],
		   unsigned char round_key[BLOCK_SIZE * ROUND_COUNT],
		   short round) {
	short i = 0, j = 0;
	for(i = 0; i < WORD_COUNT; i++) {
		for(j = 0; j < WORD_COUNT; j++) {
			//printf("XORing: %.2x with %.2x", state[i][j], round_key[(BLOCK_SIZE * round) + (i * (WORD_LENGTH_B)) + j]);
			state[i][j] ^= round_key[(BLOCK_SIZE * round) +
			    			 (i * WORD_LENGTH_B) + j];
			//printf(" resulting in %.2x\n", state[i][j]);
		}
	}
}

void sub_bytes(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT]) {
	short i = 0, j = 0;
	for(i = 0; i < WORD_COUNT; i++) {
		for(j = 0; j < WORD_COUNT; j++) {
			state[i][j] = s_box[state[i][j]];
		}
	}	
}

void shift_rows(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT]) {
	unsigned char temp = '\0';

	
	/*
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;

	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;
	*/

	
	
	temp = state[0][1];
	state[0][1] = state[1][1];
	state[1][1] = state[2][1];
	state[2][1] = state[3][1];
	state[3][1] = temp;

	temp = state[0][2];
	state[0][2] = state[2][2];
	state[2][2] = temp;
	temp = state[1][2];
	state[1][2] = state[3][2];
	state[3][2] = temp;

	temp = state[0][3];
	state[0][3] = state[3][3];
	state[3][3] = state[2][3];
	state[2][3] = state[1][3];
	state[1][3] = temp;
	

}

/*
and this is also "borrowed" from wiki
*/
unsigned char gmul(unsigned char a, unsigned char b) {
	unsigned char p = 0; /* the product of the multiplication */
	while (a && b) {
            if (b & 1) /* if b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's) */
                p ^= a; /* since we're in GF(2^m), addition is an XOR */

            if (a & 0x80) /* GF modulo: if a >= 128, then it will overflow when shifted left, so reduce */
                a = (a << 1) ^ 0x11b; /* XOR with the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) – you can change it but it must be irreducible */
            else
                a <<= 1; /* equivalent to a*2 */
            b >>= 1; /* equivalent to b // 2 */
	}
	return p;
}

void mix_columns(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT]) {
	short i = 0;
	unsigned char res[4] = {'\0'};
	for(i = 0; i < BLOCK_RC_COUNT; i++) {
	
	/*
		res[0] = (0x02 * state[0][i]) ^ (0x03 * state[1][i]) ^ state[2][i] ^ state[3][i];
		res[1] = state[0][i] ^ (0x02 * state[1][i]) ^ (0x03 * state[2][i]) ^ state[3][i];
		res[2] = state[0][i] ^ state[1][i] ^ (0x02 * state[2][i]) ^ (0x03 * state[3][i]);
		res[3] = (0x03 * state[0][i]) ^ state[1][i] ^ state[2][i] ^ (0x02 * state[3][i]);
	*/

	/*
		res[0] = (0x02 * state[i][0]) ^ (0x03 * state[i][1]) ^ state[i][2] ^ state[i][3];
		res[1] = state[i][0] ^ (0x02 * state[i][1]) ^ (0x03 * state[i][2]) ^ state[i][3];
		res[2] = state[i][0] ^ state[i][1] ^ (0x02 * state[i][2]) ^ (0x03 * state[i][3]);
		res[3] = (0x03 * state[i][0]) ^ state[i][1] ^ state[i][2] ^ (0x02 * state[i][3]);
	*/
	

	/*
		res[0] = gmul(0x02, state[0][i]) ^ gmul(0x03, state[1][i]) ^ state[2][i] ^ state[3][i];
		res[1] = state[0][i] ^ gmul(0x02, state[1][i]) ^ gmul(0x03, state[2][i]) ^ state[3][i];
		res[2] = state[0][i] ^ state[1][i] ^ gmul(0x02, state[2][i]) ^ gmul(0x03, state[3][i]);
		res[3] = gmul(0x03, state[0][i]) ^ state[1][i] ^ state[2][i] ^ gmul(0x02, state[3][i]);
	*/

	
	// working ex
		res[0] = gmul(0x02, state[i][0]) ^ gmul(0x03, state[i][1]) ^ state[i][2] ^ state[i][3];
		res[1] = state[i][0] ^ gmul(0x02, state[i][1]) ^ gmul(0x03, state[i][2]) ^ state[i][3];
		res[2] = state[i][0] ^ state[i][1] ^ gmul(0x02, state[i][2]) ^ gmul(0x03, state[i][3]);
		res[3] = gmul(0x03, state[i][0]) ^ state[i][1] ^ state[i][2] ^ gmul(0x02, state[i][3]);
	

		/*
		state[0][i] = res[0];
		state[1][i] = res[1];
		state[2][i] = res[2];
		state[3][i] = res[3];
		*/
		

		
		state[i][0] = res[0];
		state[i][1] = res[1];
		state[i][2] = res[2];
		state[i][3] = res[3];
		

		
	}
}

void rot_word(unsigned char word[WORD_LENGTH_B]) {
	unsigned char temp = '\0';

	temp = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = temp;
}

void sub_word(unsigned char word[WORD_LENGTH_B]) {
	word[0] = s_box[word[0]];
	word[1] = s_box[word[1]];
	word[2] = s_box[word[2]];
	word[3] = s_box[word[3]];
}

void key_expansion(unsigned char key[BLOCK_SIZE], unsigned char round_key[BLOCK_SIZE * ROUND_COUNT]) {
	int i = 0, j = 0;
	unsigned char last[WORD_LENGTH_B];

	// adds the original key to round_key on 0-15
	for(i = 0; i < WORD_COUNT; i++) {
		round_key[i * WORD_COUNT + 0] = key[i * WORD_COUNT + 0];
		round_key[i * WORD_COUNT + 1] = key[i * WORD_COUNT + 1];
		round_key[i * WORD_COUNT + 2] = key[i * WORD_COUNT + 2];
		round_key[i * WORD_COUNT + 3] = key[i * WORD_COUNT + 3];
	}

	for(i = WORD_COUNT; i <= WORD_COUNT * ROUND_COUNT; i++) {
		for(j = 0; j < WORD_LENGTH_B; j++) {
			last[j] = round_key[(i - 1) * WORD_LENGTH_B + j];
		}
		
		if(i % WORD_LENGTH_B == 0) {
			rot_word(last);
			sub_word(last);

			last[0] ^= Rcon[i / WORD_COUNT + 0];
			//last[1] ^= Rcon[i / WORD_COUNT + 1];
			//last[2] ^= Rcon[i / WORD_COUNT + 2];
			//last[3] ^= Rcon[i / WORD_COUNT + 3];
		} else if (WORD_COUNT > 6 && i % WORD_COUNT == 4) {
			sub_word(last);
		}

		round_key[i * WORD_COUNT + 0] = round_key[(i - WORD_LENGTH_B) * WORD_COUNT + 0] ^ last[0];
		round_key[i * WORD_COUNT + 1] = round_key[(i - WORD_LENGTH_B) * WORD_COUNT + 1] ^ last[1];
		round_key[i * WORD_COUNT + 2] = round_key[(i - WORD_LENGTH_B) * WORD_COUNT + 2] ^ last[2];
		round_key[i * WORD_COUNT + 3] = round_key[(i - WORD_LENGTH_B) * WORD_COUNT + 3] ^ last[3];
	}

	//printf("round key: %s", round_key);
	/*
	for(i = 0; i < (BLOCK_SIZE * ROUND_COUNT); i++) {
		if (!(i % 16) && i > 1) printf("\n");
		printf("%.2x ", round_key[i]);
	}
	printf("\n State: \n");
	*/
}

void print_state_aes(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT]);

void encrypt(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT], unsigned char round_key[WORD_COUNT * ROUND_COUNT]) {
	int i = 0;

	//print_state_aes(state);
	
	add_round_key(state, round_key, 0);

	//print_state_aes(state);

	for(i = 1; i <= 9; i++) {
		//if (i == 1)
		//print_state_aes(state);
		sub_bytes(state);
		//if (i == 1)
		//print_state_aes(state);
		shift_rows(state);
		//if (i == 1)
		//print_state_aes(state);
		mix_columns(state);
		//if (i == 1)
		//print_state_aes(state);
		add_round_key(state, round_key, i);
		//if (i == 1)
		//print_state_aes(state);
	}

	sub_bytes(state);
	shift_rows(state);
	add_round_key(state, round_key, i);
}

void print_state_aes(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT]) {
	short i = 0, j = 0;
	for(i = 0; i < BLOCK_RC_COUNT; i++) {
		for(j = 0; j < BLOCK_RC_COUNT; j++) {
			printf("%.2x ", state[i][j]);
		}
		printf("\n");
	}
	printf("---\n");
}

/*
void print_input() {
//	load_input("res/pokus.txt");
//	load_input("res/Shea.jpg");
	printf("size: %d\n", file_size);

//	unsigned char r1[] = {1, 1, 1, 1};
//	unsigned char r2[] = {219, 19, 83, 69};
//	unsigned char r3[] = {242, 10, 34, 92};
//	unsigned char r4[] = {198, 198, 198, 198};
	//unsigned char state[4][4] = {{1, 1, 1, 1}, {219, 19, 83, 69}, {242, 10, 34, 92}, {198, 198, 198, 198}};
	unsigned char state[4][4] = {{1, 219, 242, 198},
				     {1, 19, 10, 198},
				     {1, 83, 34, 198},
				     {1, 69, 92, 198}};
	//unsigned char state[4][4] = {{219, 19, 83, 69}, {219, 19, 83, 69}, {219, 19, 83, 69}, {219, 19, 83, 69}};
	//unsigned char state[4][] = {r1, r2, r3, r4};

	printf("state before\n");
	print_state(state);
	printf("-----------\n");
	mix_columns(state);
	printf("state after\n");
	print_state(state);
	printf("-----------\n");

//	printf("%s\n", input);
	int i = 0;
	for(i = 0; i < file_size; i++) {
		printf("%c", input[i]);
	}
}
*/

void print_output(int length) {
	int i = 0;
	
	for(i = 0; i < length; i++) {
		printf("%.2x ", output[i]);
		if(!((i + 1) % 8)) printf("\n");
	}
}

void append_state_to_output(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT], int where) {
	int i = 0, j = 0;

	for(i = 0; i < BLOCK_RC_COUNT; i++) {
		for(j = 0; j < BLOCK_RC_COUNT; j++) {
			//printf("putting %.2x @ %d\n", state[i][j], where + i * WORD_COUNT + j);
			output[where + (i * WORD_COUNT) + j] = state[i][j];
		}
	}
}

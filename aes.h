#ifndef _AES_H
#define _AES_H

/* block size in bytes */
//#define BLOCK_SIZE 16
#define MAGICAL_SIXTEEN 16
/* block row column count -> 4*4 = 16 (BLOCK_SIZE) */
//#define BLOCK_RC_COUNT 4
#define MAGICAL_FOUR 4
/* max input size = 5MiB */
#define MAX_INPUT_SIZE 41943040

#define ROUND_COUNT 11

//#define WORD_LENGTH_B 4
//#define WORD_COUNT 4

typedef unsigned char u_char;

void print_input();
void print_output(int length);
void append_state_to_output(u_char state[MAGICAL_FOUR][MAGICAL_FOUR], int where);
void key_expansion(u_char key[MAGICAL_SIXTEEN], u_char round_key[MAGICAL_SIXTEEN * ROUND_COUNT]);
void encrypt(u_char state[MAGICAL_FOUR][MAGICAL_FOUR], u_char round_key[MAGICAL_FOUR * ROUND_COUNT]);
u_char *get_output();

#endif

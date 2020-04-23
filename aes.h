#ifndef _AES_H
#define _AES_H

/* block size in bytes */
#define BLOCK_SIZE 16
/* block row column count -> 4*4 = 16 (BLOCK_SIZE) */
#define BLOCK_RC_COUNT 4
/* max input size = 4MiB - 1 (for /0) */
#define MAX_INPUT_SIZE 4194304

#define ROUND_COUNT 11
#define WORD_LENGTH_B 4
#define WORD_COUNT 4

void print_input();
void key_expansion(unsigned char key[BLOCK_SIZE], unsigned round_key[BLOCK_SIZE * ROUND_COUNT]);
void encrypt(unsigned char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT], unsigned char round_key[WORD_COUNT * ROUND_COUNT]);

#endif

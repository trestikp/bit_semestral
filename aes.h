#ifndef _AES_H
#define _AES_H

/* block size in bytes */
#define BLOCK_SIZE 16
/* block row column count -> 4*4 = 16 (BLOCK_SIZE) */
#define BLOCK_RC_COUNT 4
/* max input size = 4MiB - 1 (for /0) */
#define MAX_INPUT_SIZE 4194304

void print_input();

#endif

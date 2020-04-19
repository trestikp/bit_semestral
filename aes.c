#include "aes.h"

/* buffer for input, avoiding io during runtime */
/* *3 due to converting chars to hex in format eg: 0x[40\0] */
//char input[MAX_INPUT_SIZE][2] = {'\0'};
short input[MAX_INPUT_SIZE] = {'\0'};
//char *input;
/* block of data being encoded */
char state[BLOCK_RC_COUNT][BLOCK_RC_COUNT];

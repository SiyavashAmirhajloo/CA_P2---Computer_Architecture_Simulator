#include <stdio.h>
#include <stdbool.h>

int MainMemory[65536];
char RegisterFile[16][16];

struct instruction
{
	char hexInst[9];
	char instType; // 0->r, 1->i, 2->j
	char opcode;
	int rs, rt, rd;
	int PC;
	int rs_value;
	int rt_value;
	int rd_value;
	int imm_value;
};

void fetch(char*, struct instruction*, int);
void decode(struct instruction*);
void execute(struct instruction*, int);
void inMemory(struct instruction*);
void showDetails(char*);
void int_to_hex(char*, int);
int hexChar_to_int(char);
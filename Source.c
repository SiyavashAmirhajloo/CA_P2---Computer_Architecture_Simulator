#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Simulator.h"
#pragma warning(disable : 4996)

int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("*****Please run this program as follows:\n");
		printf("*****%s machprog.mc finalprog.txt\n", argv[0]);
		printf("*****where machprog.mc will be your machine code.\n");
		printf("and finalprog.txt will be your final answer.\n");
		exit(1);
	}
	FILE* machp;
	if ((machp = fopen(argv[1], "r")) == NULL) { // "test.mc", argv[1]
		printf(" % s cannot be opened\n", argv[1]);
		exit(1);
	}
	int line = 0;
	int pc = 0;
	int count = 0;
	fscanf(machp, "%d", &line) != '\0';
	count++;
	while (!feof(machp)) {
		fscanf(machp, "%d", &line) != '\0';
		count++;
	}
	fclose(machp);
	if ((machp = fopen(argv[1], "r")) == NULL) {
		printf(" % s cannot be opened\n", argv[1]);
		exit(1);
	}
	char** progTable;
	progTable = malloc(count * sizeof(char*));
	for (int i = 0; i < count; i++)
		progTable[i] = malloc((9) * sizeof(char));
	bool _fill = false;
	int i;
	for (i= 0; i < count && !_fill; i++) {
		fscanf(machp, "%d", &line) != '\0';
		int_to_hex(progTable[i], line);
		if (progTable[i][1] == 'e')
			_fill = true;
	}
	for (i; i < count; i++) {
		fscanf(machp, "%d", &line) != '\0';
		MainMemory[i] = line;
	}
	fclose(machp);
	struct instruction* inst = (struct instruction*)malloc(sizeof(struct instruction));
	strcpy(RegisterFile[0], "0");
	do {
		fetch(progTable[pc], inst, pc);
		decode(inst);
		execute(inst, count);
		inMemory(inst);
		if (inst->PC != pc) {
			if (inst->opcode == 'd')
				pc = inst->PC;
			else
				pc = inst->PC + 1;
		}
		else
			pc++;
		showDetails(argv[2]);
	} while (inst->opcode != 'e' && pc < count);
	return 0;
}

void fetch(char* in, struct instruction* inst, int pc)
{
	strcpy(inst->hexInst, in);
	inst->PC = pc;
	inst->opcode = in[1];
	if (inst->opcode <= '4' && inst->opcode >= '0') {
		inst->instType = 'r';
		inst->rs = hexChar_to_int(in[2]);
		inst->rt = hexChar_to_int(in[3]);
		inst->rd = hexChar_to_int(in[4]);
	}
	if (inst->opcode >= '5' && inst->opcode <= '9' || inst->opcode >= 'a' && inst->opcode <= 'c') {
		inst->instType = 'i';
		if (inst->opcode == 'b') {
			inst->rt = hexChar_to_int(in[2]);
			inst->rs = hexChar_to_int(in[3]);
		}
		else {
			inst->rs = hexChar_to_int(in[2]);
			inst->rt = hexChar_to_int(in[3]);
		}
		char temp[5];
		for (int i = 0; i < 4; i++)
			temp[i] = in[i + 4];
		temp[4] = '\0';
		inst->imm_value = (int)strtol(temp, NULL, 16);
	}
	if (inst->opcode == 'd' || inst->opcode == 'e') {
		inst->instType = 'j';
		if (inst->opcode == 'd') {
			char temp[5];
			for (int i = 0; i < 4; i++)
				temp[i] = in[i + 4];
			temp[4] = '\0';
			inst->imm_value = (int)strtol(temp, NULL, 16);
		}
	}
}

void decode(struct instruction* in)
{
	switch (in->instType)
	{
	case 'r':
		in->rs_value = atoi(RegisterFile[in->rs]);
		in->rt_value = atoi(RegisterFile[in->rt]);
		break;
	case 'i':
		if (in->opcode != '8')
			in->rs_value = atoi(RegisterFile[in->rs]);
		in->rt_value = atoi(RegisterFile[in->rt]);
		break;
	case 'j':

		break;
	default:
		break;
	}
}

void execute(struct instruction* in, int count)
{
	switch (in->opcode)
	{
	case '0':
		in->rd_value = in->rs_value + in->rt_value;
		itoa(in->rd_value, RegisterFile[in->rd], 10);
		break;
	case '1':
		in->rd_value = in->rs_value - in->rt_value;
		itoa(in->rd_value, RegisterFile[in->rd], 10);
		break;
	case '2':
		if (in->rs_value < in->rt_value)
			in->rd_value = 1;
		else
			in->rd_value = 0;
		itoa(in->rd_value, RegisterFile[in->rd], 10);
		break;
	case '3':
		in->rd_value = in->rs_value | in->rt_value;
		itoa(in->rd_value, RegisterFile[in->rd], 10);
		break;
	case '4':
		in->rd_value = !(in->rs_value & in->rt_value);
		itoa(in->rd_value, RegisterFile[in->rd], 10);
		break;
	case '5':
		if (in->hexInst[4] <= '7' && in->hexInst[4] >= '0')
			in->rt_value = in->rs_value + in->imm_value;
		if (in->hexInst[4] <= '9' && in->hexInst[4] >= '8' || in->hexInst[4] <= 'f' && in->hexInst[4] >= 'a')
			in->rt_value = in->rs_value + in->imm_value - 65536;
		itoa(in->rt_value, RegisterFile[in->rt], 10);
		break;
	case '6':
		if (in->hexInst[4] <= '7' && in->hexInst[4] >= '0') {
			if (in->rs_value < in->imm_value)
				in->rt_value = 1;
			else
				in->rt_value = 0;
		}
		if (in->hexInst[4] <= '9' && in->hexInst[4] >= '8' || in->hexInst[4] <= 'f' && in->hexInst[4] >= 'a') {
			if (in->rs_value < in->imm_value - 65536)
				in->rt_value = 1;
			else
				in->rt_value = 0;
		}
		itoa(in->rt_value, RegisterFile[in->rt], 10);
		break;
	case '7':
		in->rt_value = in->rs_value | in->imm_value;
		itoa(in->rt_value, RegisterFile[in->rt], 10);
		break;
	case '8':
		in->rt_value = in->imm_value * pow(2, 16);
		itoa(in->rt_value, RegisterFile[in->rt], 10);
		break;
	case '9':
		break;
	case 'a':
		break;
	case 'b':
		if (in->rs_value == in->rt_value) {
			if (in->hexInst[4] <= '7' && in->hexInst[4] >= '0')
				in->PC += in->imm_value;
			if (in->hexInst[4] <= '9' && in->hexInst[4] >= '8' || in->hexInst[4] <= 'f' && in->hexInst[4] >= 'a')
				in->PC += (in->imm_value - 65536);
		}
		break;
	case 'c':
		in->rt_value = in->PC + 1;
		in->PC = in->rs_value;
		break;
	case 'd':
		if (in->hexInst[4] <= '7' && in->hexInst[4] >= '0')
			in->PC = in->imm_value;
		if (in->hexInst[4] <= '9' && in->hexInst[4] >= '8' || in->hexInst[4] <= 'f' && in->hexInst[4] >= 'a')
			in->PC = (in->imm_value - 65536);
		break;
	case 'e':
		break;
	default:
		break;
	}
}

void inMemory(struct instruction* in)
{
	switch (in->opcode)
	{
	case '9':
		in->rt_value = MainMemory[in->rs_value + in->imm_value];
		itoa(in->rt_value, RegisterFile[in->rt], 10);
		break;
	case 'a':
		MainMemory[in->rs_value + in->imm_value] = in->rt_value;
		break;
	default:
		break;
	}
}

void showDetails(char* argv)
{
	FILE* machp;
	if ((machp = fopen(argv, "w+")) == NULL) {
		printf("%s cannot be opened\n", argv[2]);
		exit(1);
	}
	printf("*Register File*\n\n");
	if (fprintf(machp, "*Register File*\n\n") < 0) {
		fclose(machp);
		perror("Couldn't write to it");
		fprintf(stderr, "errno:  %d\n", errno);
		exit(1);
	}
	for (int i = 0; i < 16; i++) {
		if (strcmp(RegisterFile[i], "") != 0)
			printf("%d:\t%s\n", i, RegisterFile[i]);
		else
			printf("%d:\tnull\n", i);
		if (fprintf(machp, "%d:\t%s\n", i, RegisterFile[i]) < 0) {
			fclose(machp);
			perror("Couldn't write to it");
			fprintf(stderr, "errno:  %d\n", errno);
			exit(1);
		}
	}
	printf("---------------\n");
	if (fprintf(machp, "---------------\n") < 0) {
		fclose(machp);
		perror("Couldn't write to it");
		fprintf(stderr, "errno:  %d\n", errno);
		exit(1);
	}
	fclose(machp);
	printf("for continue press Enter.\n");
	char ch;
	scanf("%c", &ch);
}

void int_to_hex(char* hexOfInst, int inst)
{
	
	char temp[9];
	int i = 0;
	sprintf(temp, "%x", inst);
	int len = strlen(temp);
	if (len < 8) {
		for (i; i < 8 - len; i++)
			hexOfInst[i] = '0';
	}
	for (i; i < 8; i++)
		hexOfInst[i] = temp[i - 8 + len];
	hexOfInst[8] = '\0';
}

int hexChar_to_int(char h)
{
	if (h >= '0' && h <= '9')
		return h - '0';
	if (h >= 'A' && h <= 'F')
		return h - 'A' + 10;
	if (h >= 'a' && h <= 'f')
		return h - 'a' + 10;
	return -1;
}

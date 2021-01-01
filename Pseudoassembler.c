#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Const.h"
#include"Parser.h"
#include"Order.h"
#include"Label_Offset.h"
#include"Executor.h"
#include"Visuals.h"

#include"curses.h"

#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif


char path[PATH_SIZE];
char tc;
char code_type[LABEL_SIZE];
char tout[LABEL_SIZE];
int p_m = 0; //p_m = 0 - kod pseudoassemblera, p_m = 1 - kod maszynowy
int k, k1;
int file_length, debug;
FILE* fp;
FILE* output_file;

void Transfer(FILE* fp);
void Write(int size, int ver, int mark);

int main(int argc, char** argv) {

	//Wczytanie pliku
	if (argc > 1) {
		fp = fopen(argv[1], "r");

		if (fp == NULL) {
			printf("Path error");
			exit(EXIT_FAILURE);
		}
	}
	else {
		printf("No arguments");
		exit(EXIT_FAILURE);
	}

	if (argc > 2) {
		if (strcmp(argv[2], "psa_code") == 0) p_m = 0;
		if (strcmp(argv[2], "msck_code") == 0) p_m = 1;
	}

	if (argc > 3 && k != 3) {
		if (strcmp(argv[3], "debug") == 0) debug = 1;
	}

	Transfer(fp);

	if (p_m == 0) {
		Parse(file_length);
		Convert_arg2(file_length);
	}
	else {
		Parse_MC(file_length);
	}

	Init_Memory(file_length);

	if(p_m == 0)
	Write(file_length, 0, p_m);

	Decode(file_length, debug);

	Write(file_length, 1, p_m);

	WriteMemory();

	fclose(fp);
	End();

	return 0;
}


void Transfer(FILE* fp) { //przenosi ka¿d¹ liniê z pliku wejœciowego na miejsce w tablicy struktury "row"
	k = 0;
	int IsSpace = 0;

	while (fgets(temp_line, LINE_SIZE, fp) != NULL) {

		if (temp_line[0] != '/' && IsSpace == 0) {
			strcpy(row[k].line, temp_line);
			k++;
		}
	}

	file_length = k;
}

void Write(int size, int ver, int mark) {

	//tworzenie pliku wyjœcia
	if(ver==0)
		output_file = fopen(OUTPUT_FILENAME1, "w");
	else
		output_file = fopen(OUTPUT_FILENAME2, "w");

	if (output_file == NULL) {
		perror("Could not create an output file");
		exit(EXIT_FAILURE);
	}

	for (k = 0; k < memlen; k++) {

		if (mem[k].dir == 1) {
			fprintf(output_file, "~~ ~~ ~~ ~~\n");
		}
		else {
			sprintf(tout, "%08X", mem[k].val);
			fprintf(output_file, "%c%c %c%c %c%c %c%c\n", tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6], tout[7]);
		}
	}

	k = 0;
	while (row[k].type == 0) k++;

	for (; k < size; k++) {

		if (row[k].label[0] == '\n')
			fprintf(output_file, "\n");
		else{
			//rozkaz
			fprintf(output_file, "%2X ", row[k].cmdcode);
			fprintf(output_file, "%01X%01X", row[k].r1, row[k].r2);

			if (row[k].order[1] != 'R') {
				sprintf(tout, "%04X", row[k].move);
				fprintf(output_file, " %c%c %c%c", tout[0], tout[1], tout[2], tout[3]);
			}

			if (k != size - 1)
				fprintf(output_file, "\n");
		}
	}

	fclose(output_file);
}
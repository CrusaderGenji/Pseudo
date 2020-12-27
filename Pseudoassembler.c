#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Const.h"
#include"Parser.h"
#include"Order.h"
#include"Label_Offset.h"
#include"Executor.h"

#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif


char path[PATH_SIZE];
char tout[LABEL_SIZE];
int k, k1;
int file_length;
FILE* fp;
FILE* output_file;

void Transfer(FILE* fp);
void Write(int size);

int main(int argc, char** argv) {

	//Wczytanie pliku
	{
		if (argc>1) {
			fp = fopen(argv[1], "r");

			if (fp == NULL)
				printf("\nPath error.\n");
			else
				k++;
		}

		if(k == 0)
		for (;;) {
			printf("File path required. Please put in a correct path to the desired text file. . .\n");
			scanf("%s", path);
			fp = fopen(path, "r");

			if (fp == NULL)
				printf("\nPath error.\n");
			else
				if (path == "") {
					perror("");
					exit(EXIT_FAILURE);
				}
				else
					break;		
		}
	}

	

	Transfer(fp);

	//printf("%d\n\r", file_length);

	Parse(file_length);

	Convert_arg2(file_length);

	Convert_Code(file_length);

	for(k=0; k<file_length; k++)
		printf("%d | %10s | %2s | %5s | %5s| %d | %d | %d | %d | %2x || %d | %d | %d\n\r", row[k].type, row[k].label, row[k].order, row[k].arg1, row[k].arg2, row[k].r1, row[k].r2, row[k].move, row[k].size, row[k].cmdcode, row[k].direct, row[k].number, row[k].val);

	//Print_Labels();

	Write(file_length, 0);

	Decode(file_length);

	Write(file_length, 1);

	fclose(fp);

	return 0;
}


void Transfer(FILE* fp) { //przenosi ka¿d¹ liniê z pliku wejœciowego na miejsce w tablicy struktury "row"
	k = 0;

	while (fgets(temp_line, LINE_SIZE, fp) != NULL) {
		strcpy(row[k].line, temp_line);
		k++;
	}

	file_length = k;
}

void Write(int size) {

	//tworzenie pliku wyjœcia
	output_file = fopen(OUTPUT_FILENAME, "w");

	if (output_file == NULL) {
		perror("Could not create an output file");
		exit(EXIT_FAILURE);
	}

	for (k = 0; k < memlen; k++) {
		sprintf(tout, "%08X", mem[k]);
		fprintf(output_file, "%c%c %c%c %c%c %c%c\n", tout[0], tout[1], tout[2], tout[3], tout[4], tout[5], tout[6], tout[7]);
	}

	for (k = 0; k < size; k++) {

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
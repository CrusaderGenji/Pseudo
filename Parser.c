#include "Parser.h"


int IsNumber(char x) {

	if (x>='0' && x<='9')
		return 1;
	
	return 0;
}

void Parse(int size) {

	sw = 0;

	for (i = 0; i < size; i++) {
		li = 0;

		strncpy(temp_line, "", strlen(temp_line));
		strcpy(temp_line, row[i].line);


		if (temp_line[0] == '\n') {
			sw = i;
			row[i].type = 2;
			row[i].label[0] = '\n';
			continue;
		}

		if (temp_line[0] == '/') {
			continue;
		}

		// etykieta, je�li jest
		if (temp_line[li] == ' ' || temp_line[li] == '\t')
			while (temp_line[li] == ' ' || temp_line[li] == '\t' && li < LINE_SIZE)li++;
		else
			while (temp_line[li] != ' ' && temp_line[li] != '\t' && li < LINE_SIZE) {
				row[i].label[li] = temp_line[li];
				li++;
			}

		if (strcmp(row[i].label, "")!=0)
			Create_Label(row[i].label, i);

		while (temp_line[li] == ' ' || temp_line[li] == '\t')
			li++;


		//sw zapisuje lini� podzia�u programu na sekcj� danych i sekcj� rozkaz�w
		if(sw==0){//dyrektywa
			
			//zapisanie typu komendy i samej komendy
			row[i].type = 0;
			row[i].order[0] = temp_line[li];
			row[i].order[1] = temp_line[li+1];

			li++;

			//sprawdzenie, czy dyrektywa jest dyrektyw� DS czy DC
			if (temp_line[li] == 'C')
				row[i].direct = 0;
			else
				if(temp_line[li] == 'S')
					row[i].direct = 1;
				else {
					perror("Directive read failure");
					exit(EXIT_FAILURE);
				}
			li++;


			while (temp_line[li] == ' ' || temp_line[li] == '\t') {
				li++;
			}

			//je�li podana jest liczba sta�ych/miejsc do alokacji
			if (IsNumber(temp_line[li]) == 1) {
				j = 0;
				while (IsNumber(temp_line[li]) == 1 && li < LINE_SIZE) {
					row[i].arg1[j] = temp_line[li];
					li++; j++;
				}

				row[i].number = atoi(row[i].arg1);
				
			}

			//je�li nie zosta�a podana liczba sta�ych, przyjmij 1 na pole ilo�ci sta�ych
			if (strcmp(row[i].arg1, "")==0)
				row[i].number = 1;


			//je�li zosta�a podana dyrektywa DC, znajd� i zapisz podan� przez nia warto�� sta�ej
			if (row[i].direct == 0) {
				while (IsNumber(temp_line[li]) != 1 && li < LINE_SIZE)
					li++;

				if (temp_line[li - 1] == '-') {
					row[i].arg2[0] = '-';
					j = 1;
				}else j = 0;
				
				while (IsNumber(temp_line[li]) == 1 && li < LINE_SIZE) {
					row[i].arg2[j] = temp_line[li];
					li++; j++;
				}

				row[i].val = atoi(row[i].arg2);
			}

			row[i].size = LONG_COMM * row[i].number;
		}
		else{//rozkaz

			//zapis oznaczenia rozkazu
			row[i].type = 1;
			row[i].order[0] = temp_line[li];
			li++;

			if (temp_line[li] >= 'A' && temp_line[li] <= 'Z') {
				row[i].order[1] = temp_line[li];
				li++;
			}

			while (temp_line[li] == ' ' || temp_line[li] == '\t' && li < LINE_SIZE)
				li++;

			//zapis argument�w (adres zawsze zapisuje si� w arg2)
			if (row[i].order[0] != 'J') {
				j = 0;
				while (temp_line[li] != ' ' && temp_line[li] != ',' && temp_line[li] != '\t' && li < LINE_SIZE) {
					row[i].arg1[j] = temp_line[li];
					j++; li++;
				}

				while (temp_line[li] == ' ' || temp_line[li] == ',' || temp_line[li] == '\t' && li < LINE_SIZE)
					li++;
			}

			j = 0;
			while (temp_line[li] != ' ' && temp_line[li] != '\n' && temp_line[li] != '\t' && temp_line[li] !='\\' && li < LINE_SIZE) {
				row[i].arg2[j] = temp_line[li];
				j++; li++;
			}

			//dla wszystkich rozkaz�w opr�cz rozkaz�w JUMP, arg1 przechowuje numer pierwszego rejestru 
			if (row[i].order[0] != 'J')
				row[i].r1 = atoi(row[i].arg1);

			//dla wszystkich rozkaz�w typu rejestr-rejestr, arg2 przechowuje numer drugiego rejestru
			if (row[i].order[1] == 'R')
				row[i].r2 = atoi(row[i].arg2);


			//konwersja �a�cucha rozkazu na odpowiadaj�cy mu numer w systemie 16-tkowym
			row[i].cmdcode = CommandCode(row[i].order);
			if (row[i].cmdcode == 0) {
				perror("Command read failure");
				exit(EXIT_FAILURE);
			}

			//rozmiar komendy
			if (row[i].order[1]=='R')
				row[i].size = SHORT_COMM;
			else
				row[i].size = LONG_COMM;

		}


	}
}

void Parse_MC(int size) {
	li = 0;
	char temp_mc[STRING_SIZE];
	char temp_nat_line[LINE_SIZE];

	while (row[li].line[0] != '\n') {
		row[li].type = 0;
		row[li].order[0] = 'D';

		if (row[li].line[0] == '~') {

			row[li].order[1] = 'S';
			row[li].direct = 1;
			row[li].number = 1;
			row[li].val = 0;
		}
		else {
			row[li].order[1] = 'C';
			row[li].direct = 0;
			row[li].number = 1;
			//konwersja liczby zapisanej w kodzie maszynowym na liczbe typu int
			temp_mc[0] = row[li].line[0];
			temp_mc[1] = row[li].line[1];
			temp_mc[2] = row[li].line[3];
			temp_mc[3] = row[li].line[4];
			temp_mc[4] = row[li].line[6];
			temp_mc[5] = row[li].line[7];
			temp_mc[6] = row[li].line[9];
			temp_mc[7] = row[li].line[10];

			printf("%10s, %8s\n", row[li].line, temp_mc);
			row[li].val = strtol(temp_mc, NULL, 16);
		}
		li++;
	}

	row[li].type = 2;
	li++;

	while (li < size) {
		row[li].type = 1;

		char temp_ord[2];
		temp_ord[0] = row[li].line[0];
		temp_ord[1] = row[li].line[1];
		row[li].cmdcode = strtol(temp_ord, NULL, 16);
		row[li].order[0] = temp_ord[0];
		row[li].order[1] = temp_ord[1];

		char 

	}

}
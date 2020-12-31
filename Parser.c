#include "Parser.h"


int IsNumber(char x) {

	if (x>='0' && x<='9')
		return 1;
	
	return 0;
}

int char2hex(char x) {

	if (x >= '0' && x <= '9')
		return (int)(x - '0');
	else
		return (int)(x - 'A' + 10);

}

void Parse(int size) {

	sw = 0;
	css = 0; dss = 0;
	int position = 0;

	for (i = 0; i < size;i++) {
		li = 0;

		strncpy(temp_line, "", strlen(temp_line));
		strcpy(temp_line, row[i].line);


		if (temp_line[0] == '\n') {
			sw = i;
			row[i].type = 2;
			row[i].label[0] = '\n';
			position = 0;
			continue;
		}

		if (temp_line[0] == '/') {
			continue;
		}

		// etykieta, jeœli jest
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


		//sw zapisuje liniê podzia³u programu na sekcjê danych i sekcjê rozkazów
		if(sw==0){//dyrektywa
			
			//zapisanie typu komendy i samej komendy
			row[i].type = 0;
			row[i].order[0] = temp_line[li];
			row[i].order[1] = temp_line[li+1];

			li++;

			//sprawdzenie, czy dyrektywa jest dyrektyw¹ DS czy DC
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

			//jeœli podana jest liczba sta³ych/miejsc do alokacji
			if (IsNumber(temp_line[li]) == 1) {
				j = 0;
				while (IsNumber(temp_line[li]) == 1 && li < LINE_SIZE) {
					row[i].arg1[j] = temp_line[li];
					li++; j++;
				}

				row[i].number = atoi(row[i].arg1);
				
			}

			//jeœli nie zosta³a podana liczba sta³ych, przyjmij 1 na pole iloœci sta³ych
			if (strcmp(row[i].arg1, "")==0)
				row[i].number = 1;


			//jeœli zosta³a podana dyrektywa DC, znajdŸ i zapisz podan¹ przez nia wartoœæ sta³ej
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

			//zapis wielkoœci danej i jej pozycji wzglêdem pocz¹tku sekcji danych
			row[i].size = LONG_COMM * row[i].number;
			row[i].pos = position;
			position += row[i].size;
			dss++;
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

			//zapis argumentów (adres jest zawsze zapisywany w arg2)
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
			while (temp_line[li] != ' ' && temp_line[li] != '\n' && temp_line[li] != '\t' && temp_line[li] !='/' && li < LINE_SIZE) {
				row[i].arg2[j] = temp_line[li];
				j++; li++;
			}

			//dla wszystkich rozkazów oprócz rozkazów JUMP, arg1 przechowuje numer pierwszego rejestru 
			if (row[i].order[0] != 'J')
				row[i].r1 = atoi(row[i].arg1);

			//dla wszystkich rozkazów typu rejestr-rejestr, arg2 przechowuje numer drugiego rejestru
			if (row[i].order[1] == 'R')
				row[i].r2 = atoi(row[i].arg2);

			//printf("%s\n", row[i].order);

			//konwersja ³añcucha rozkazu na odpowiadaj¹cy mu numer w systemie 16-tkowym
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

			//zapis pozycji rozkazu wzglêdem pocz¹tku sekcji rozkazów
			row[i].pos = position;
			position += row[i].size;
			css++;
		}
	}
}

void Parse_MC(int size) {
	li = 0;
	dss = 0; css = 0;
	char temp_mc[STRING_SIZE];
	char temp_nat_line[LINE_SIZE];
	int position = 0;

	//dyrektywy
	while (row[li].line[0] != '\n') {

		if (row[li].line[0] == '/')
			continue;

		row[li].type = 0;
		row[li].order[0] = 'D';
		row[li].size = LONG_COMM;
		row[li].pos = position;
		position += row[li].size;

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
			i = 0, j = 0;
			while (row[li].line[j] != '\n' && i < STRING_SIZE) {
				if (row[li].line[j] != ' ') {
					temp_mc[i] = row[li].line[j];
					i++, j++;
				}
				else
					j++;
			}

			//printf("%10s, %s,  ", row[li].line, temp_mc);
			row[li].val = strtol(temp_mc, NULL, 16);
			//printf("%d\n", row[li].val);
		}
		li++;
		dss++;
		strncpy(temp_mc, "", STRING_SIZE);
	}

	//oznaczenie wiersza dziel¹cego sekcje
	row[li].type = 2;
	row[li].label[0] = '\n';
	position = 0;
	li++;

	//rozkazy
	while (li < size) {
		if (row[li].line[0] == '/')
			continue;

		row[li].type = 1;
		i = 0;
		char temp_ord[2] = "";
		temp_ord[0] = row[li].line[i];
		i++;
		while (row[li].line[i] == ' ' || row[li].line[i] == '\t') i++;
		temp_ord[1] = row[li].line[i];
		row[li].cmdcode = strtol(temp_ord, NULL, 16);
		//printf("%0X  ", row[li].cmdcode);
		
		//wpisanie w strukturê row[].order kodu rozkazu w kodzie naturalnym
		RevCommandCode(row[li].cmdcode, row[li].order);
		//printf("%s\n", row[li].order);

		i++;

		//zczytanie numerów rejestrów których u¿ywa dany rozkaz
		while (row[li].line[i]==' ' || row[li].line[i] == '\t') i++;

		row[li].r1 = char2hex(row[li].line[i]);
		i++;

		while (row[li].line[i] == ' ' || row[li].line[i] == '\t') i++;

		row[li].r2 = char2hex(row[li].line[i]);
		i++;

		//Jeœli rozkaz jest rozkazem d³ugim, uwzglêdnij 3 i 4 bajt komendy
		if (IsNumber(row[li].line[0]) == 0) {

			row[li].size = LONG_COMM;

			while (row[li].line[i] == ' ' || row[li].line[i] == '\t') i++;

			j = i;
			i = 0;

			while (row[li].line[j] != '\n' && i < STRING_SIZE) {
				if (row[li].line[j] != ' ' && row[li].line[j] != '\t') {
					temp_mc[i] = row[li].line[j];
					i++, j++;
				}
				else
					j++;
			}

			row[li].move = strtol(temp_mc, NULL, 16);
			strncpy(temp_mc, "", STRING_SIZE);
		}
		else
			row[li].size = SHORT_COMM;

		//zapis pozycji rozkazu wzglêdem pocz¹tku sekcji rozkazów
		row[li].pos = position;
		position += row[li].size;
		strncpy(temp_ord, "", ORDER_SIZE);
		css++;
		li++;
	}
}
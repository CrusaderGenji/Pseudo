#include"Label_Offset.h"

#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

void Create_Label(const char* z, int it) {

	for (o = 0; strcmp(labels[o].text, "") != 0 && o < MAX_LABEL_AMOUNT;)
		o++;

	strcpy(labels[o].text, z);

	it--; //poniewa¿ nie wliczamy rozmiaru obecnej linii, tylko poprzednich

	for (; it >= 0; it--)
		labels[o].move += row[it].size;

	//warunki poprawnoœci etykiet
	if (IsNumber(z[0]) == 1) {
		perror("Label name processing error - Label cannot start with a number\n");
		exit(EXIT_FAILURE);
	}
	/*
	if (CommandCode(z) != 0) {
		perror("Label name processing error - Command symbols cannot be used as labels\n");
		exit(EXIT_FAILURE);
	}
	
	it = strlen(z);

	for (o = 0; o < it; o++) {
		if (IsNumber(z[o]) == 0 && (z[o] < 'A' || z[o]>'Z')) {
			perror("Label name processing error - Only alphanumeric symbols can be used in labels\n");
				exit(EXIT_FAILURE);
		}
	}
	*/
}

int Find_Label(int z1) {

	for (o1 = 0; o1<MAX_LABEL_AMOUNT ; o1++) {

		if (strcmp(labels[o1].text, row[z1].arg2) == 0)
			return labels[o1].move;
		else
			if (strcmp(labels[o1].text, "")==0)
				return -1;
	}

	return -1;
}

void Convert_arg2(int size) {

	for (o = 0; o < sw; o++)
		data_section_size += row[o].size;

	for (o = 0; o < size; o++) {

		if(strcmp(row[o].arg2, "") != 0 && row[o].type==1)
			if (IsNumber(row[o].arg2[0]) == 0) {
				//Jeœli przesuniêcie odwo³uje siê do etykiety
				row[o].move = Find_Label(o);
				row[o].r2 = MEM_REG;

				if (row[o].move == -1)
					perror("Label read failure");
			}
			else {
				//funkcja Convert_arg2() wykonuje siê po funkcji Parse(), a Parse() domyœlnie przepisuje numer drugiego rejestru dla rozkazów typu rej-rej
				if (row[o].order[1] != 'R') {
					argit = 0;

					memset(temp, 0, LABEL_SIZE);

					while (IsNumber(row[o].arg2[argit]) == 1) {
						temp[argit] = row[o].arg2[argit];
						argit++;
					}

					row[o].r2 = MEM_REG;
					row[o].move = atoi(temp);

					if (row[o].arg2[argit] == '(') {
						argit++;
						memset(temp, 0, LABEL_SIZE);

						temp[0] = row[o].arg2[argit];
						argit++;

						if (IsNumber(row[o].arg2[argit]) == 1)
							temp[1] = row[o].arg2[argit];

						row[o].r2 = atoi(temp);
						if (row[o].r2 > 15) {
							perror("Wrong register given");
							exit(EXIT_FAILURE);
						}
					}
				}
			}

		//Poprawka przesuniêcia dla rozkazu skoku
		if (row[o].order[0] == 'J') {
			row[o].r2 = COMM_REG;
			row[o].move -= data_section_size;
		}

	}
}


void Print_Labels() {

	o = 0;
	while (strcmp(labels[o].text, "") != 0 && o<100) {
		printf("%d %s %d\n\r", o, labels[o].text, labels[o].move);
		o++;
	}

	printf("\n\rThat is all...\n\r");
}
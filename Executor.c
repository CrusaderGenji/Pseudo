#include"Executor.h"

void Convert_Code(int size) {

	memlen = 0;
	int it = 0;
	r[15] = 0;
	while (row[it].type != 2) {
		memlen += row[it].number;
		printf("%d\n", memlen);
		it++;
	}
	
	//memlen - iloœæ wierszy sekcji danych w tekœcie programu

	if (memlen != 0) {
		//mem - wskaŸnik tablicy zawieraj¹cej dane zadeklarowane przez u¿ytkownika w tekœcie programu
		mem = (struct MEMFRAG*)malloc(memlen*sizeof(struct MEMFRAG));

		if (mem == NULL) {
			perror("Insufficient memory. Cannot run program");
			exit(EXIT_FAILURE);
		}
		else {
			printf("Zaalokowano\n");
			//alokacja pamiêci
			it = 0;
			int memit = 0;
			while (row[it].type != 2) {
				//"value" - wartoœæ do przepisania (dla DS funkcja wpisze w komórkê wartoœæ 0)
				int value = 0;
				value = row[it].val;

				for (int ilosc = 0; ilosc < row[it].number; ilosc++) {
					mem[memit].val = value;
					printf("%d %d\n", mem[memit].val, value);

					if (row[it].direct == 1)
						mem[memit].dir = 1;

					memit++;
				}
				it++;
			}
		}
	}

	it++;
	mid = it;
}

void End() {
	free(mem);
}

void Decode(int size) {

	int it = mid;

	while (it < size) {
		//sprawdzam, do której grupy nale¿y rozkaz
		int temp_cmd;
		temp_cmd = row[it].cmdcode;
		temp_cmd /= 16;

		if (temp_cmd == 0x1 || temp_cmd == 0xD) {
			//funkcje arytmetyczne
			printf("ARITHETIC\n");
			it = Ary(it);
		}
		else {
			if (temp_cmd == 0xE) {
				//skoki
				printf("JUMP\n");
				it = Jump(it);

			}
			else {
				//zapisywanie wartoœci
				printf("LOAD/STORE\n");
				it = Load_Store(it);

			}
		}
	}
}

void Set_PSR(int value) {

	if (value > 0)
		psr = 1;

	if (value == 0)
		psr = 0;

	if (value < 0)
		psr = 2;

	printf("%d\n", psr);
}

int CRA(int shift, int regis) {
	//Convert Register Address (komenda do konwersji przesuniêcia,
	//ujednolica przypadki w których rozkaz odnosi siê do miejsca w pamiêci wskazywanego przez wartoœæ w rejestrze)

	return (shift + r[regis])/LONG_COMM;
}

int Ary(int rit) {

	int ordnum, tpos;

	ordnum = row[rit].cmdcode;
	ordnum %= 16;

	if (row[rit].order[1] == 'R')
		tpos = r[row[rit].r2];
	else
		tpos = mem[CRA(row[rit].move, row[rit].r2)].val;

	if (row[rit].r2 == COMM_REG) {
		perror("Error - Pointed to value from the command register");
		exit(EXIT_FAILURE);
	}

	//dodawanie
	if (ordnum == 0 || ordnum == 1) {
		printf("%d + %d\n", r[row[rit].r1], tpos);
		r[row[rit].r1] = r[row[rit].r1] + tpos;
		printf("A %d\n  ", r[row[rit].r1]);
		Set_PSR(r[row[rit].r1]);
		return ++rit;
	}

	//odejmowanie
	if (ordnum == 2 || ordnum == 3) {
		printf("%d - %d\n", r[row[rit].r1], tpos);
		r[row[rit].r1] = r[row[rit].r1] - tpos;
		printf("S %d\n  ", r[row[rit].r1]);
		Set_PSR(r[row[rit].r1]);
		return ++rit;
	}

	//mno¿enie
	if (ordnum == 4 || ordnum == 5) {
		printf("%d * %d\n", r[row[rit].r1], tpos);
		r[row[rit].r1] = r[row[rit].r1] * tpos;
		printf("M %d  ", r[row[rit].r1]);
		Set_PSR(r[row[rit].r1]);
		return ++rit;
	}

	//dzielenie
	if (ordnum == 6 || ordnum == 7) {

		//zwróæ b³¹d w przypadku dzielenia przez 0
		if (tpos == 0) {
			perror("Error - Division by 0");
			exit(EXIT_FAILURE);
		}

		printf("%d / %d\n", r[row[rit].r1], tpos);
		r[row[rit].r1] = r[row[rit].r1] / tpos;
		printf("D %d  ", r[row[rit].r1]);
		Set_PSR(r[row[rit].r1]);
		return ++rit;
	}
	
	//porównanie
	if (ordnum == 8 || ordnum == 9) {

		int tcomp;

		tcomp = r[row[rit].r1] - tpos;
		printf("C %d  ", tcomp);
		Set_PSR(tcomp);
		return ++rit;
	}

	perror("Error - arythmetic order could not be executed");
	exit(EXIT_FAILURE);
}

int Jump(int rit) {

	int ordnum;
	ordnum = row[rit].cmdcode;
	ordnum %= 16;

	if (ordnum == 0 || (ordnum == 1 && psr == 0) || (ordnum == 2 && psr == 1) || (ordnum == 3 && psr == 2)) {
		int jmp = row[rit].move;
		int dest = mid;

		while (jmp > 0) {
			jmp -= row[dest].size;
			dest++;
		}

		printf("JY, %d\n", dest);
		return dest;
	}
	else {
		printf("JN\n");
		return ++rit;
	}
}

int Load_Store(int rit) {

	int ordnum;
	int sh = CRA(row[rit].move, row[rit].r2);
	int reg1 = row[rit].r1;

	ordnum = row[rit].cmdcode;
	ordnum %= 16;

	//Load
	if (ordnum == 0) {
		r[reg1] = mem[sh].val;
		printf("L\n");
		return ++rit;
	}

	//Load Register
	if (ordnum == 1) {
		r[reg1] = r[row[rit].r2];
		printf("LR\n");
		return ++rit;
	}

	//Load Address
	if (ordnum == 2) {
		r[reg1] = sh;
		printf("LA\n");
		return ++rit;
	}

	//Store
	if (ordnum == 3) {
		mem[sh].val = r[reg1];
		printf("ST\n");
		return ++rit;
	}

	perror("Error - load/store order could not be executed");
	exit(EXIT_FAILURE);
}
#include"Executor.h"

void Init_Memory(int size) { //funkcja 

	memlen = 0;
	int it = 0;
	r[14] = row[dss].pos;
	r[15] = 0;
	while (row[it].type != 2) {
		memlen += row[it].number;
		//printf("%d\n", memlen);
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
			//printf("Zaalokowano\n");
			//alokacja pamiêci
			it = 0;
			int memit = 0;
			mem[0].pos = 0;
			while (row[it].type != 2) {
				//"value" - wartoœæ do przepisania (dla DS funkcja wpisze w komórkê wartoœæ 0)
				int value = 0;
				value = row[it].val;

				if (strcmp(row[it].label, "") != 0)
					strcpy(mem[memit].label, row[it].label);
				else
					strcpy(mem[memit].label, "/");

				for (int ilosc = 0; ilosc < row[it].number; ilosc++) {
					mem[memit].val = value;
					
					if(ilosc > 0)
						strcpy(mem[memit].label, "/");

					if (memit != 0)
						mem[memit].pos = mem[memit - 1].pos + 4;

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
	return;
}

void WriteMemory() {
	int mit;

	for (mit = 0; mit < 16; mit++)
		printf("REG %2d - %d\n", mit, r[mit]);

	for (mit = 0; mit < memlen; mit++)
		printf("%4d: %d\n", mit * 4, mem[mit].val);

	return;
}

void End() {
	free(mem);
	return;
}

void Decode(int size, int sbs) { //funkcja wykonuj¹ca rozkazy pobrane z programu, zapisane w strukturze row[]

	int it = mid;
	it--;
	int it2;
	if (sbs == 1) {
		InitVis();
	}

	while (it < size) {
		//sprawdzam, do której grupy nale¿y rozkaz
		it2 = it;
		int temp_cmd;
		temp_cmd = row[it].cmdcode;
		temp_cmd /= 16;

		if (temp_cmd == 0x1 || temp_cmd == 0xD) {
			//funkcje arytmetyczne
			it = Ary(it);
		}
		else {
			if (temp_cmd == 0xE) {
				//skoki
				it = Jump(it, size);
			}
			else {
				//zapisywanie wartoœci
				it = Load_Store(it, size);
			}
		}

		if (sbs == 1)
			sbs = Visualize(it2, it, size);

		if (it >= size && sbs == 1) {
			EndVis();
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

	if (row[rit].r2 < 0 || row[rit].r2 > 15 || CRA(row[rit].move, row[rit].r2) > memlen) {
		perror("Error - Memory access failure");
		exit(EXIT_FAILURE);
	}

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
		if ((r[row[rit].r1] > 0 && tpos > INT_MAX - r[row[rit].r1]) || (r[row[rit].r1] < 0 && tpos < INT_MIN - r[row[rit].r1])) {
			perror("Error - Overflow");
			exit(EXIT_FAILURE);
		}
		else {
			r[row[rit].r1] = r[row[rit].r1] + tpos;
			Set_PSR(r[row[rit].r1]);
		}

		return ++rit;
	}

	//odejmowanie
	if (ordnum == 2 || ordnum == 3) {
		if ((r[row[rit].r1] > 0 && tpos < INT_MIN + r[row[rit].r1]) || (r[row[rit].r1] < 0 && tpos - 1 > INT_MAX + r[row[rit].r1])) {
			perror("Error - Overflow");
			exit(EXIT_FAILURE);
		}
		else {
			r[row[rit].r1] = r[row[rit].r1] - tpos;
			Set_PSR(r[row[rit].r1]);
		}

		return ++rit;
	}

	//mno¿enie
	if (ordnum == 4 || ordnum == 5) {
		if ((tpos == -1 && r[row[rit].r1] == INT_MIN) || (r[row[rit].r1] == -1 && tpos == INT_MIN) || (tpos != 0 && (r[row[rit].r1] > INT_MAX / tpos || r[row[rit].r1] < INT_MIN / tpos))) {
			perror("Error - Overflow");
			exit(EXIT_FAILURE);
		}
		else {
			r[row[rit].r1] = r[row[rit].r1] * tpos;
			Set_PSR(r[row[rit].r1]);
		}

		return ++rit;
	}

	//dzielenie
	if (ordnum == 6 || ordnum == 7) {

		//zwróæ b³¹d w przypadku dzielenia przez 0
		if (tpos == 0) {
			perror("Error - Division by 0");
			exit(EXIT_FAILURE);
		}

		if ((tpos == -1 && r[row[rit].r1] == INT_MIN) || (r[row[rit].r1] == -1 && tpos == INT_MIN)) {
			perror("Error - Overflow");
			exit(EXIT_FAILURE);
		}
		else {
			r[row[rit].r1] = r[row[rit].r1] / tpos;
			Set_PSR(r[row[rit].r1]);
		}

		return ++rit;
	}
	
	//porównanie
	if (ordnum == 8 || ordnum == 9) {
		if ((r[row[rit].r1] > 0 && tpos < INT_MIN + r[row[rit].r1]) || (r[row[rit].r1] < 0 && tpos - 1 > INT_MAX + r[row[rit].r1])) {
			perror("Error - Overflow");
			exit(EXIT_FAILURE);
		}
		else {
		int tcomp;
		tcomp = r[row[rit].r1] - tpos;
		Set_PSR(tcomp);
		}

		return ++rit;
	}

	perror("Error - arythmetic order could not be executed");
	exit(EXIT_FAILURE);
}

int Jump(int rit, int length) {

	int ordnum;
	ordnum = row[rit].cmdcode;
	ordnum %= 16;

	if (row[rit].r2 < 0 || row[rit].r2 > 15 || CRA(row[rit].move, row[rit].r2) > length) {
		perror("Error - Memory access failure");
		exit(EXIT_FAILURE);
	}

	if (ordnum == 0 || (ordnum == 1 && psr == 0) || (ordnum == 2 && psr == 1) || (ordnum == 3 && psr == 2)) {
		int jmp = row[rit].move;
		int dest = mid;

		while (jmp != row[dest].pos) {
			dest++;
		}

		return dest;
	}
	else
		return ++rit;
}

int Load_Store(int rit, int length) {

	int ordnum;
	int sh = CRA(row[rit].move, row[rit].r2);
	int reg1 = row[rit].r1;

	if (row[rit].r1 < 0 || row[rit].r1 > 15 || sh > length) {
		perror("Error - Memory access failure");
		exit(EXIT_FAILURE);
	}

	ordnum = row[rit].cmdcode;
	ordnum %= 16;

	//Load
	if (ordnum == 0) {
		r[reg1] = mem[sh].val;
		//printf("L\n");
		return ++rit;
	}

	//Load Register
	if (ordnum == 1) {
		r[reg1] = r[row[rit].r2];
		//printf("LR\n");
		return ++rit;
	}

	//Load Address
	if (ordnum == 2) {
		r[reg1] = sh;
		//printf("LA\n");
		return ++rit;
	}

	//Store
	if (ordnum == 3) {
		mem[sh].val = r[reg1];
		mem[sh].dir = 0;
		//printf("ST\n");
		return ++rit;
	}

	perror("Error - load/store order could not be executed");
	exit(EXIT_FAILURE);
}
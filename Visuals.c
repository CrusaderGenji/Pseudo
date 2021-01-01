#include"Visuals.h"

#define SPACE 1
#define BW 1 //border width
#define PSA_W 29
#define MSCK_W 19
#define REG_W 17
#define REG_H 16
#define MEM_W 29
#define ALL_W 8*BW + PSA_W + MSCK_W + REG_W + MEM_W + 2
#define ALL_H 30
#define DESC_H 4
#define DESC_W REG_W + 2*BW + 1 + MEM_W
#define TITLE_H 1
#define SIGN_H 1
#define L1 TITLE_H + 1
#define L2 DESC_H + 2*BW + 1
#define L3 L2 + TITLE_H
#define L4 L3 + REG_H +2*BW
#define L5 L4 + TITLE_H
#define S2 PSA_W + 2*BW + 1 + MSCK_W + 2*BW
#define S3 S2 + REG_W + 2 * BW
#define MEM_H 20
#define COMM_LINES ALL_H-2*BW

//bor - border; con - content
WINDOW *desc_bor, *desc_con;
WINDOW *psa_bor, *psa_con, *psa_title;
WINDOW *msck_bor, *msck_con, *msck_title;
WINDOW *reg_bor, *reg_con, *reg_title;
WINDOW *sign_bor, *sign_con, *sign_title;
WINDOW *mem_bor, *mem_con, *mem_title;

int Visualize(int step, int step2, int size) {
	int c, mpos, mpos2, mhigh, mhigh2;
	//step2--;
	Code(step - dss -1, step2, size);
	PrintPSR(row[step2].pos);
	if (row[step].order[1] == 'R' || row[step].order[0] == 'J') {
		mpos = 0;
		mhigh = -1;
		PrintMemory(mpos, mhigh);
	}
	else {
		mpos = (row[step].move + r[row[step].r2])/ 4;
		mhigh = mpos;
		PrintMemory(mpos, mhigh);
	}
	mhigh2 = mhigh; mpos2 = mpos;
	HighlightREG(step);
	Refreshing();
	while (1) {
		c = getch();
		
		if (c == (int)(KEY_DOWN) || c == (int)('n')) {
			if (mpos2 + 1 < memlen) {
				mpos2++;
				mhigh2++;
			}
			PrintMemory(mpos2, mhigh2);
			wrefresh(mem_con);
		}

		if (c == (int)(KEY_UP) || c == (int)('m')) {
			if (mpos2 - 1 >= 0) {
				mpos2--;
				mhigh2--;
			}
			PrintMemory(mpos2, mhigh2);
			wrefresh(mem_con);
		}

		if (c == (int)('c')) {
			PrintMemory(mpos, mhigh);
			wrefresh(mem_con);
		}

		if (c == (int)('z') || c == (int)('Z')) return 0;
		if (c == (int)('x') || c == (int)('X')) return 1;
	}

	
}

void HighlightStep(int step) {
	//kolory podœwietlonej linii kodu pseudoassemblera
	mvwchgat(psa_con, step, 0, 14, COLOR_PAIR(5), 0, NULL);
	mvwchgat(psa_con, step, 14, 3, COLOR_PAIR(2), 0, NULL);
	mvwchgat(psa_con, step, 17, 1, COLOR_PAIR(5), 0, NULL);
	mvwchgat(psa_con, step, 18, 11, COLOR_PAIR(3), 0, NULL);
	//mvwchgat(psa_con, step, 28, 1, COLOR_PAIR(5), 0, NULL);
	//kolory podœwietlonej linii kodu maszynowego
	mvwchgat(msck_con, step, 0, 9, COLOR_PAIR(5), 0, NULL);
	mvwchgat(msck_con, step, 9, 2, COLOR_PAIR(2), 0, NULL);
	mvwchgat(msck_con, step, 11, 9, COLOR_PAIR(3), 0, NULL);
	//mvwchgat(msck_con, step, 18, 1, COLOR_PAIR(5), 0, NULL);
	return;
}

void HighlightREG(int step) {
	mvwchgat(reg_con, row[step].r2, 0, REG_W, COLOR_PAIR(3), 0, NULL);
	mvwchgat(reg_con, row[step].r1, 0, REG_W, COLOR_PAIR(2), 0, NULL);
	return;
}

void HighlightMEM(int fr) {
	mvwchgat(mem_con, fr, 0, MEM_W, COLOR_PAIR(3), 0, NULL);
	return;
}

void MarkNextOrder(int nstep) {

	mvwchgat(psa_con, nstep, 0, PSA_W, COLOR_PAIR(4), 0, NULL);
	mvwchgat(msck_con, nstep, 0, MSCK_W, COLOR_PAIR(4), 0, NULL);
	return;
}

void Code(int st, int mark, int size) {

	int i, o;
	werase(psa_con);
	werase(msck_con);

	if (st < 14 || css < (COMM_LINES)) {
		for (i = 0, o = 0; i < min(COMM_LINES, css); i++, o++) PrintCode(o, i);
		HighlightStep(st);
	}
	else
		if (css - st + 1 < 14) {
			for (i = css+1 - 28, o = 0; i < css+1; i++, o++) PrintCode(o, i);
			HighlightStep(28 + st - css - 1);
		}
		else {
			for (i = st - (COMM_LINES) / 2, o = 0; i < st + (COMM_LINES) / 2; i++, o++) PrintCode(o, i);
			HighlightStep(14);
		}

	return;
}

void PrintCode(int pi, int pmi) {
	//kod pseudoassemblera
	mvwprintw(psa_con, pi, 0, "%s", row[dss + 1 + pmi].label);
	mvwprintw(psa_con, pi, LABEL_SIZE + 1, "%s", row[dss + 1 + pmi].order);

	mvwprintw(psa_con, pi, LABEL_SIZE + 4, "%2d,", row[dss + 1 + pmi].r1);

	if (strcmp(row[dss + 1 + pmi].arg1, "") != 0 && row[dss + 1 + pmi].order[1] != 'R')
		mvwprintw(psa_con, pi, LABEL_SIZE + 8, "%s", row[dss + 1 + pmi].arg2);
	else
		mvwprintw(psa_con, pi, LABEL_SIZE + 8, "%d(%d)", row[dss + 1 + pmi].move, row[dss + 1 + pmi].r2);
	//kod maszynowy
	mvwprintw(msck_con, pi, 0, "%4d: ", row[dss + 1 + pmi].pos);
	mvwprintw(msck_con, pi, 7, "%2X %01X%01X", row[dss + 1 + pmi].cmdcode, row[dss + 1 + pmi].r1, row[dss + 1 + pmi].r2);

	if (row[dss + 1 + pmi].order[1] != 'R') {
		char tchr[LABEL_SIZE];
		sprintf(tchr, "%04X", row[dss + 1 + pmi].move);
		mvwprintw(msck_con, pi, 12, " %c%c %c%c", tchr[0], tchr[1], tchr[2], tchr[3]);
	}

	return;
}

void PrintPSR(int position) {
	werase(sign_con);
	refresh(sign_con);
	int psr2 = psr * 4;

	mvwprintw(sign_con, 0, 0, "0000%X000 0000%04X", psr2, position);

	return;
}

void PrintRegisters() {

	int ri = 0;
	char rchr[LABEL_SIZE];
	for (; ri < COMM_REG; ri++){
		sprintf(rchr, "%08X", r[ri]);
		mvwprintw(reg_con, ri, 0, "%3d: %c%c %c%c %c%c %c%c ", ri, rchr[0], rchr[1], rchr[2], rchr[3], rchr[4], rchr[5], rchr[6], rchr[7]);
	}

	mvwprintw(reg_con, 14, 0, " 14: Commands    ");
	mvwprintw(reg_con, 15, 0, " 15: Data        ");
	return;
}

void PrintMemory(int frag, int act) {
	
	if (act >=0) {
		if (frag < MEM_H / 2 || memlen < (MEM_H)) {
			for (i = 0, o = 0; i < min(MEM_H, memlen); i++, o++) PrintMEMFrag(i, o);
			HighlightMEM(act);
		}
		else
			if (memlen - frag < MEM_H / 2) {
				for (i = memlen - (MEM_H), o = 0; i <= memlen; i++, o++) PrintMEMFrag(i, o);
				HighlightMEM(MEM_H + act - memlen);
			}
			else {
				for (i = frag - (MEM_H) / 2, o = 0; i <= frag + (MEM_H) / 2; i++, o++) PrintMEMFrag(i, o);
				HighlightMEM(MEM_H/2);
			}
	}
	else {
		if (frag < MEM_H / 2 || memlen < (MEM_H)) {
			for (i = 0, o = 0; i < min(MEM_H, memlen); i++, o++) PrintMEMFrag(i, o);
		}
		else
			if (memlen - frag < MEM_H / 2) {
				for (i = memlen - (MEM_H), o = 0; i <= memlen; i++, o++) PrintMEMFrag(i, o);
			}
			else {
				for (i = frag - (MEM_H) / 2, o = 0; i <= frag + (MEM_H) / 2; i++, o++) PrintMEMFrag(i, o);
			}
	}
	
	return;
}

void PrintMEMFrag(int mi, int pl) {//wykorzystywane przez PrintMemory(), drukuje wskazan¹ przez "mi" liniê na miejscu "pl" w sekcji danych

	char mchr[LABEL_SIZE];

	if (mem[mi].dir == 1)
		sprintf(mchr, "~~~~~~~~");
	else
		sprintf(mchr, "%08X", mem[mi].val);

	mvwprintw(mem_con, pl, 0, "%4d: ", mem[mi].pos);

	if (strcmp(mem[mi].label, "/") != 0)
		mvwprintw(mem_con, pl, 6, "%10s %c%c %c%c %c%c %c%c ", mem[mi].label, mchr[0], mchr[1], mchr[2], mchr[3], mchr[4], mchr[5], mchr[6], mchr[7]);
	else
		mvwprintw(mem_con, pl, 6, "           %c%c %c%c %c%c %c%c ", mchr[0], mchr[1], mchr[2], mchr[3], mchr[4], mchr[5], mchr[6], mchr[7]);
}

void InitColors() {
	start_color();
	init_pair(3, 7, 88);
	init_pair(2, 7, 22);
	init_pair(4, 7, 236);
	init_pair(5, 7, 239);
	return;
}

void InitVis() {

	initscr();
	noecho();
	cbreak();
	resize_term(ALL_H, ALL_W);
	InitWindows();
	InitColors();
	keypad(stdscr, TRUE);
	wcolor_set(desc_con, COLOR_PAIR(2), NULL);
	Refreshing();

	return;
}

void InitWindows() {
	psa_title = newwin(TITLE_H, PSA_W+2*BW, 0, 1);
	psa_bor = newwin(ALL_H - TITLE_H, PSA_W + 2*BW, 1, 1);
	box(psa_bor, 0, 0);
	psa_con = newwin(ALL_H - TITLE_H - 2*BW, PSA_W, 1 + BW, BW + 1);

	msck_title = newwin(TITLE_H, MSCK_W + 2 * BW, 0, PSA_W + 2 * BW + SPACE);
	msck_bor = newwin(ALL_H - TITLE_H, MSCK_W + 2*BW, 1, PSA_W + 2 * BW + SPACE);
	box(msck_bor, 0, 0);
	msck_con = newwin(ALL_H - TITLE_H - 2 * BW, MSCK_W, L1, PSA_W + 2 * BW + SPACE + BW);

	desc_bor = newwin(DESC_H + 2 * BW, DESC_W + 2 * BW, 0, S2);
	desc_con = newwin(DESC_H+1, DESC_W, BW, S2 + BW);

	reg_title = newwin(TITLE_H, REG_W + 1, L2, S2);
	reg_bor = newwin(REG_H + 2 * BW, REG_W + 2*BW, L3, S2);
	box(reg_bor, 0, 0);
	reg_con = newwin(REG_H, REG_W, L3 + BW, S2 + BW);

	sign_title = newwin(TITLE_H, REG_W + 2 * BW, L4, S2);
	sign_bor = newwin(SIGN_H + 2 * BW, REG_W + 2 * BW, L5, S2);
	box(sign_bor, 0, 0);
	sign_con = newwin(SIGN_H, REG_W, L5 + BW, S2 + BW);

	mem_title = newwin(TITLE_H, MEM_W + 2 * BW, L2, S3);
	mem_bor = newwin(ALL_H - 8, MEM_W + 2 * BW, L3, S3);
	box(mem_bor, 0, 0);
	mem_con = newwin(ALL_H - 8 - 2*BW, MEM_W, L3 + BW, S3 + BW);

	return;
}

void RefreshBorders() {
	wrefresh(psa_bor);
	wrefresh(msck_bor);
	wrefresh(desc_bor);
	wrefresh(reg_bor);
	wrefresh(sign_bor);
	wrefresh(mem_bor);
	return;
}

void StaticElements() {
	RefreshBorders();
	mvwprintw(psa_title, 0, (PSA_W + 2 - strlen("Pseudoassembler code"))/2, "Pseudoassembler code");
	mvwprintw(msck_title, 0, (MSCK_W + 2 - strlen("Machine code")) / 2, "Machine code");
	mvwprintw(desc_bor, 0, (DESC_W + 2 - strlen("INTERPRETER PSEUDOASSEMBLERA")) / 2, "INTERPRETER PSEUDOASSEMBLERA");
	mvwprintw(desc_con, 0, 0, "'X' to advance a step\n UP and DOWN arrow keys or 'n' and 'm'\n to navigate the Data Section\n'C' to view currently used data fragment\n'Z' to exit dss: %3d css: %3d", dss, css);
	mvwprintw(sign_title, 0, (REG_W + 2 - strlen("Program state")) / 2, "Program state");
	mvwprintw(reg_title, 0, (REG_W + 2 - strlen("Registers")) / 2, "Registers");
	mvwprintw(mem_title, 0, (MEM_W + 2 - strlen("Data section")) / 2, "Data section");
	return;
}

void Refreshing() {
	refresh();
	wrefresh(psa_title);
	wrefresh(psa_con);
	wrefresh(msck_title);
	wrefresh(msck_con);
	wrefresh(desc_con);
	wrefresh(reg_title);
	wrefresh(reg_con);
	wrefresh(sign_title);
	wrefresh(sign_con);
	wrefresh(mem_title);
	wrefresh(mem_con);
	StaticElements();
	PrintRegisters();
	return;
}

void EndVis() {

	int c1, pos1, mh1;
	pos1 = 0; mh1 = 0;
	Refreshing();
	wattrset(desc_con, COLOR_PAIR(3));
	mvwprintw(desc_con, 0, 0, "                                               ");
	mvwprintw(desc_con, 0, 0, " Code processing finished. Press Z to exit. . .");
	wrefresh(desc_con);

	while (1) {
		c1 = getch();

		if (c1 == (int)(KEY_DOWN) || c1 == (int)('n')) {
			if (pos1 + 1 < memlen) {
				pos1++;
				mh1++;
			}
			PrintMemory(pos1, mh1);
			wrefresh(mem_con);
		}

		if (c1 == (int)(KEY_UP) || c1 == (int)('m')) {
			if (pos1 - 1 >= 0) {
				pos1--;
				mh1--;
			}
			PrintMemory(pos1, mh1);
			wrefresh(mem_con);
		}

		if (c1 == (int)('z') || c1 == (int)('Z')) break;
	}

	endwin();
	return;
}
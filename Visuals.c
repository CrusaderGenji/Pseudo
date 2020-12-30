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
#define MEM_H ALL_H - L3

//bor - border; con - content
WINDOW *desc_bor, *desc_con;
WINDOW *psa_bor, *psa_con, *psa_title;
WINDOW *msck_bor, *msck_con, *msck_title;
WINDOW *reg_bor, *reg_con, *reg_title;
WINDOW *sign_bor, *sign_con, *sign_title;
WINDOW *mem_bor, *mem_con, *mem_title;

int Visualize(int step, int size) {
	int c;

	Code(step, size);
	Refreshing();
	while (1) {
		c = getchar();
		if (c == (int)('z') || c == (int)('Z')) return 0;
		if (c == (int)('x') || c == (int)('X')) return 1;
	}

	
}

void Code(int step, int size) {

	int i;

	if (step < 10 || css < (ALL_H - 2 * BW)) {

		for (i = 0; i < min(ALL_H - 2 * BW, css); i++) PrintCode(i);
	}
	else
		if (css - step < 15) {
			for (i = size - (ALL_H - 2 * BW); i < size; i++) PrintCode(i);
		}
		else {
			for (i = size - (ALL_H - 2 * BW); i < size; i++) PrintCode(i);
		}
	return;
}

void PrintCode(int pi) {
	//kod pseudoassemblera
	mvwprintw(psa_con, pi, 0, "%s", row[dss + 1 + pi].label);
	mvwprintw(psa_con, pi, LABEL_SIZE + 1, "%s", row[dss + 1 + pi].order);

	mvwprintw(psa_con, pi, LABEL_SIZE + 4, "%2d,", row[dss + 1 + pi].r1);

	if (strcmp(row[dss + 1 + pi].arg1, "")!=0 && row[dss + 1 + pi].order[1] != 'R')
		mvwprintw(psa_con, pi, LABEL_SIZE + 8, "%s", row[dss + 1 + pi].arg2);
	else
		mvwprintw(psa_con, pi, LABEL_SIZE + 8, "%d(%d)", row[dss + 1 + pi].move, row[dss + 1 + pi].r2);
	//kod maszynowy
	mvwprintw(msck_con, pi, 0, "%4d: ", row[dss + 1 + pi].pos);
	mvwprintw(msck_con, pi, 7, "%2X %01X%01X", row[dss + 1 + pi].cmdcode, row[dss + 1 + pi].r1, row[dss + 1 + pi].r2);

	if (row[dss + 1 + pi].order[1] != 'R') {
		char tchr[LABEL_SIZE];
		sprintf(tchr, "%04X", row[dss + 1 + pi].move);
		mvwprintw(msck_con, pi, 12, " %c%c %c%c", tchr[0], tchr[1], tchr[2], tchr[3]);
	}

	return;
}

void PrintRegisters() {

	int ri = 0;
	char rchr[LABEL_SIZE];
	for (; ri < COMM_REG; ri++){
		sprintf(rchr, "%08X", r[ri]);
		mvwprintw(reg_con, ri, 0, "%3d: %c%c %c%c %c%c %c%c", ri, rchr[0], rchr[1], rchr[2], rchr[3], rchr[4], rchr[5], rchr[6], rchr[7]);
	}

	mvwprintw(reg_con, 14, 0, " 14: Commands");
	mvwprintw(reg_con, 15, 0, " 15: Data");
	return;
}

void PrintMemory() {

	int mi;
	char mchr[LABEL_SIZE];

	for (mi = 0; mi < memlen; mi++) {

		if (mem[mi].dir == 1)
			sprintf(mchr, "~~~~~~~~");
		else
			sprintf(mchr, "%08X", mem[mi].val);

		mvwprintw(mem_con, mi, 0, "%4d: %5s %c%c %c%c %c%c %c%c", mem[mi].pos, mem[mi].label, mchr[0], mchr[1], mchr[2], mchr[3], mchr[4], mchr[5], mchr[6], mchr[7]);
	}

	return;
}

void InitColors() {
	start_color();
	//init_color();

}

void InitVis() {

	initscr();
	noecho();
	cbreak();
	//keypad();
	resize_term(ALL_H, ALL_W);
	InitWindows();
	InitColors();
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
	msck_con = newwin(ALL_H - TITLE_H - 10 - 2 * BW, MSCK_W, L1, PSA_W + 2 * BW + SPACE + BW);

	desc_bor = newwin(DESC_H + 2 * BW, DESC_W + 2 * BW, 0, S2);
	desc_con = newwin(DESC_H, DESC_W, BW, S2 + BW);

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
	//mvwprintw(psa_con, 0, 0, "PSA Content");
	mvwprintw(msck_title, 0, (MSCK_W + 2 - strlen("Machine code")) / 2, "Machine code");
	//mvwprintw(msck_con, 0, 0, "MSCK Content");
	mvwprintw(desc_bor, 0, (DESC_W + 2 - strlen("INTERPRETER PSEUDOASSEMBLERA")) / 2, "INTERPRETER PSEUDOASSEMBLERA");
	mvwprintw(desc_con, 0, 0, "Press X to advance a step\nPress Z to exit");
	mvwprintw(sign_title, 0, (REG_W + 2 - strlen("Program state")) / 2, "Program state");
	mvwprintw(sign_con, 0, 0, "SIGN Content");
	mvwprintw(reg_title, 0, (REG_W + 2 - strlen("Registers")) / 2, "Registers");
	mvwprintw(reg_con, 0, 0, "0123456789ABCDEF");
	mvwprintw(mem_title, 0, (MEM_W + 2 - strlen("Data section")) / 2, "Data section");
	mvwprintw(mem_con, 0, 0, "0123456789ABCDEFGHIJKLMNOP");
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
	PrintMemory();
	return;
}

void EndVis() {

	int c;
	Refreshing();
	werase(desc_con);
	mvwprintw(desc_con, 1, 1, "Code finished\nPress Z to exit. . .");

	while (1) {
		c = getchar();
		if (c == (int)('z') || c == (int)('Z')) break;
	}

	endwin();
	return;
}
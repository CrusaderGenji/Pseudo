#include"Visuals.h"

WINDOW* top_border, *top_content;
WINDOW* psa_border, * psa_content, * psa_title;

void InitVis() {
	initscr();
	noecho();
	cbreak();
	//keypad();


}

void EndVis() {
	endwin();
	return;
}

//pads for scrollable shit. prefresh() for 'em
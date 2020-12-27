#include"Const.h"


int CommandCode(const char* ord) {

	if (strcmp(ord, "AR") == 0)
		return 0x10;
	if (strcmp(ord, "A") == 0)
		return 0xD1;
	if (strcmp(ord, "SR") == 0)
		return 0x12;
	if (strcmp(ord, "S") == 0)
		return 0xD3;
	if (strcmp(ord, "MR") == 0)
		return 0x14;
	if (strcmp(ord, "M") == 0)
		return 0xD5;
	if (strcmp(ord, "DR") == 0)
		return 0x16;
	if (strcmp(ord, "D") == 0)
		return 0xD7;
	if (strcmp(ord, "CR") == 0)
		return 0x18;
	if (strcmp(ord, "C") == 0)
		return 0xD9;
	if (strcmp(ord, "J") == 0)
		return 0xE0;
	if (strcmp(ord, "JZ") == 0)
		return 0xE1;
	if (strcmp(ord, "JP") == 0)
		return 0xE2;
	if (strcmp(ord, "JN") == 0)
		return 0xE3;
	if (strcmp(ord, "L") == 0)
		return 0xF0;
	if (strcmp(ord, "LR") == 0)
		return 0x31;
	if (strcmp(ord, "LA") == 0)
		return 0xF2;
	if (strcmp(ord, "ST") == 0)
		return 0xF3;

	return 0;
}

void RevCommandCode(const char* ord, char* target) {

	if (strcmp(ord, "10") == 0) {
		strcpy(target, "AR");
		return;
	}
	if (strcmp(ord, "D1") == 0) {
		strcpy(target, "A");
		return;
	}
	if (strcmp(ord, "12") == 0) {
		strcpy(target, "SR");
		return;
	}
	if (strcmp(ord, "D3") == 0) {
		strcpy(target, "S");
		return;
	}
	if (strcmp(ord, "14") == 0) {
		strcpy(target, "MR");
		return;
	}
	if (strcmp(ord, "D5") == 0) {
		strcpy(target, "M");
		return;
	}
	if (strcmp(ord, "16") == 0) {
		strcpy(target, "DR");
		return;
	}
	if (strcmp(ord, "D7") == 0) {
		strcpy(target, "D");
		return;
	}
	if (strcmp(ord, "18") == 0) {
		strcpy(target, "CR");
		return;
	}
	if (strcmp(ord, "D9") == 0) {
		strcpy(target, "C");
		return;
	}
	if (strcmp(ord, "E0") == 0) {
		strcpy(target, "J");
		return;
	}
	if (strcmp(ord, "E1") == 0) {
		strcpy(target, "JZ");
		return;
	}
	if (strcmp(ord, "E2") == 0) {
		strcpy(target, "JP");
		return;
	}
	if (strcmp(ord, "E3") == 0) {
		strcpy(target, "JN");
		return;
	}
	if (strcmp(ord, "F0") == 0) {
		strcpy(target, "L");
		return;
	}
	if (strcmp(ord, "31") == 0) {
		strcpy(target, "LR");
		return;
	}
	if (strcmp(ord, "F2") == 0) {
		strcpy(target, "LA");
		return;
	}
	if (strcmp(ord, "F3") == 0) {
		strcpy(target, "ST");
		return;
	}
}
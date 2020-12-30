#include"Const.h"
#include"Order.h"


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

void RevCommandCode(int ord, char* target) {

	if (ord==0x10) {
		strcpy(target, "AR");
		return;
	}
	if (ord==0xD1) {
		strcpy(target, "A");
		return;
	}
	if (ord==0x12) {
		strcpy(target, "SR");
		return;
	}
	if (ord==0xD3) {
		strcpy(target, "S");
		return;
	}
	if (ord==0x14) {
		strcpy(target, "MR");
		return;
	}
	if (ord==0xD5) {
		strcpy(target, "M");
		return;
	}
	if (ord==0x16) {
		strcpy(target, "DR");
		return;
	}
	if (ord==0xD7) {
		strcpy(target, "D");
		return;
	}
	if (ord==0x18) {
		strcpy(target, "CR");
		return;
	}
	if (ord==0xD9) {
		strcpy(target, "C");
		return;
	}
	if (ord==0xE0) {
		strcpy(target, "J");
		return;
	}
	if (ord==0xE1) {
		strcpy(target, "JZ");
		return;
	}
	if (ord==0xE2) {
		strcpy(target, "JP");
		return;
	}
	if (ord==0xE3) {
		strcpy(target, "JN");
		return;
	}
	if (ord==0xF0) {
		strcpy(target, "L");
		return;
	}
	if (ord==0x31) {
		strcpy(target, "LR");
		return;
	}
	if (ord==0xF2) {
		strcpy(target, "LA");
		return;
	}
	if (ord==0xF3) {
		strcpy(target, "ST");
		return;
	}

	perror("Command not found");
	exit(EXIT_FAILURE);
}
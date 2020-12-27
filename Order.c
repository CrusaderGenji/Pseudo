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

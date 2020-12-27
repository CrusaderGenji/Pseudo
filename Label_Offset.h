#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Const.h"
#include"Parser.h"
#include"Order.h"


int o, o1, argit;
int data_section_size;
char temp[LABEL_SIZE];

struct LABEL {
	char text[LABEL_SIZE];
	int move;

} labels[MAX_LABEL_AMOUNT];

void Create_Label(char* z, int it);
int Find_Label(int z1);
void Convert_arg2(int size);
void Print_Labels();
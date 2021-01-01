#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "Const.h"
#include "Parser.h"
#include "Label_Offset.h"
#include "Visuals.h"

struct MEMFRAG { //Memory Fragment - komórka pamiêci - dir przechowuje dane o formie zapisu 
	//(jeœli dir = 0, jest tam zapisana liczba, natomiast jeœli dir =1, jest to miejsce zaalokowane przez DS bez przypisanej przez u¿ytkownika wartoœci) 
	int dir;
	long int val;
	int pos;
	char label[LABEL_SIZE];
};

struct MEMFRAG *mem;
int memlen; //*mem - wskaŸnik do tablicy zawieraj¹cej dane zadeklarowane sekcj¹ danych, memsize - suma zarezerwowanych komórek pamiêci (wielkoœæ tablicy mem)
long int r[MEM_REG+1]; //rejestry
int psr; //program state register - rejestr stanu programu
//psr przyjmuje: 0 kiedy wynik operacji jest 0, 1 kiedy wynik jest pozytywny, 2 kiedy wynik jest negatywny, 3 w przypadku b³êdu
int mid;

void Init_Memory(int);
void WriteMemory();
void End();
void Decode(int, int);
void Set_PSR(int);
int CRA(int, int);
int Ary(int);
int Jump(int, int);
int Load_Store(int, int);

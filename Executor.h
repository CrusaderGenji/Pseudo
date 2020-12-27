#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Const.h"
#include "Parser.h"
#include "Label_Offset.h"

struct MEMFRAG { //Memory Fragment - komórka pamiêci - dir przechowuje dane o formie zapisu 
	//(jeœli dir = 0, jest tam zapisana liczba, natomiast jeœli dir =1, jest to miejsce zaalokowane przez DS bez przypisanej przez u¿ytkownika wartoœci) 
	int dir;
	int val;
};

int *mem, memlen; //*mem - wskaŸnik do tablicy zawieraj¹cej dane zadeklarowane sekcj¹ danych, memsize - suma zarezerwowanych komórek pamiêci (wielkoœæ tablicy mem)
int r[MEM_REG+1]; //rejestry
int psr; //program state register - rejestr stanu programu
//psr przyjmuje: 0 kiedy wynik operacji jest 0, 1 kiedy wynik jest pozytywny, 2 kiedy wynik jest negatywny, 3 w przypadku b³êdu
int mid;

void Convert_Code(int);
void Decode(int);
int Ary(int);
void Set_PSR(int);
int CRA(int, int);
int Jump(int);
int Load_Store(int);

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Const.h"
#include "Parser.h"
#include "Label_Offset.h"

struct MEMFRAG { //Memory Fragment - kom�rka pami�ci - dir przechowuje dane o formie zapisu 
	//(je�li dir = 0, jest tam zapisana liczba, natomiast je�li dir =1, jest to miejsce zaalokowane przez DS bez przypisanej przez u�ytkownika warto�ci) 
	int dir;
	int val;
};

int *mem, memlen; //*mem - wska�nik do tablicy zawieraj�cej dane zadeklarowane sekcj� danych, memsize - suma zarezerwowanych kom�rek pami�ci (wielko�� tablicy mem)
int r[MEM_REG+1]; //rejestry
int psr; //program state register - rejestr stanu programu
//psr przyjmuje: 0 kiedy wynik operacji jest 0, 1 kiedy wynik jest pozytywny, 2 kiedy wynik jest negatywny, 3 w przypadku b��du
int mid;

void Convert_Code(int);
void Decode(int);
int Ary(int);
void Set_PSR(int);
int CRA(int, int);
int Jump(int);
int Load_Store(int);

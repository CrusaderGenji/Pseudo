#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Const.h"
#include"Order.h"
#include"Label_Offset.h"

#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

int i, j, li; //li - line iterator
int sw; //indykator parsera wskazuj�cy czy funkcja ma dzia�a� wed�ug wskaza� dla linii dyrektywy, czy rozkazu
//Wskazuje tak�e na pust� lini� mi�dzy sekcj� danych a sekcj� rozkaz�w
char temp_line[LINE_SIZE]; //tymczasowe przechowywanie �ancucha ca�ej linii


struct LINE {

	int type;//0 - sekcja danych, 1 - sekcja rozkaz�w, 2 - pusty wiersz mi�dzy sekcjami
	char label[LABEL_SIZE];
	char line[LINE_SIZE];
	char order[4];
	int pos; //position - liczba w bajtach okre�laj�ca pozycj� rozkazu w pami�ci; liczone osobno dla sekcji rozkaz�w i sekcji danych

	//if command
	char arg1[LABEL_SIZE];
	char arg2[LABEL_SIZE];
	int r1, r2;
	int move; //przesuniecie
	int size; //dlugosc rozkazu - 2 lub 4 bajty
	int cmdcode; //kod komendy (w systemie szesnastkowym)

	//if directive
	int direct; //0 - DC, 1 - DS
	int number; //ilosc zadeklarowanych zmiennych (domy�lnie 1)
	int val; //wartosc stalej w przypadku DC
};

struct LINE row[MAX_LINE_AMOUNT];

int IsNumber(char);
int char2hex(char);
void Parse(int);
void Parse_MC(int);

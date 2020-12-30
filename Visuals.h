#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include"Const.h"
#include"Parser.h"
#include"Executor.h"
#include"curses.h"

int Visualize(int, int);
void Code(int, int);
void PrintCode(int);
void PrintRegisters();
void PrintMemory();
void InitVis();
void InitWindows();
void StaticElements();
void RefreshBorders();
void Refreshing();
void EndVis();

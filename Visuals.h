#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include"Const.h"
#include"Parser.h"
#include"Executor.h"
#include"curses.h"

int Visualize(int, int, int);
void HighlightStep(int);
void HighlightREG(int);
void HighlightMEM(int);
void MarkNextOrder(int);
void Code(int, int, int);
void PrintCode(int, int);
void PrintPSR(int);
void PrintRegisters();
void PrintMemory(int, int);
void PrintMEMFrag(int, int);
void InitColors();
void InitVis();
void InitWindows();
void StaticElements();
void RefreshBorders();
void Refreshing();
void EndVis();

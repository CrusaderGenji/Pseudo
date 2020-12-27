#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Const.h"
#include"Label_Offset.h"

#if defined( _WIN32 )
#pragma warning(disable:4996)
#endif

int CommandCode(const char* ord);
void RevCommandCode(const char* ord, char* target);
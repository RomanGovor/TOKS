#pragma once
#include <iostream>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define maxBuff 200

int readFromFile(HANDLE hFile, char* buffer);
void loading();
bool writeToFile(HANDLE hFile, char* buffer);
void processingPorts(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork);
void endWork(std::string s, int r);
int setBaudrate();
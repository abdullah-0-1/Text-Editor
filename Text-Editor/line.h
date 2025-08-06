#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include"Cursor.h"
using namespace std;
class Line
{
	char* Cs;
	int size;

public:
	int MaxCs = 20;
	// ***********CONSTRUCTORS*********
	Line();
	Line(const Line& L);
	Line(const char* str, int l);
	Line(const char* str);

	// ***********DESTRUCTOR********
	~Line();
	// ***********FUNCTIONS*************
	void InsertCAt(char c, int ind);
	void Append(char c);
	void DeleteCAt(int ind);
	int GetSize();
	char GetCharAt(int ind);
	char* ToUpper(char* C, int l);
	char ToUpper(char c);
	char* ToLower(char* C, int l);
	char ToLower(char c);
	vector<int> Substr(char* sub, int l);
	void PrintCs()const;
	void Indent(Cursor& cur);
	void UnIndent(Cursor& cur);
	bool IsDeli(const int& cur)const;
	void Trim(int Nsize);
	Line* Split(int p);
	void MergeLines(const Line& L);
	void DeepCopy(const Line& L);
	void Clear();
	void ToggleCase(int ind);
	void DeleteFromCursorToEnd(int ind);
	int FindNextWordStart(int startPos) const;
	int FindPrevWordStart(int startPos) const;
};

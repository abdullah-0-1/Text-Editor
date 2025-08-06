#pragma once
#include <iostream>
#include<vector>
#include <windows.h>
#include <conio.h>
#include"Line.h"
using namespace std;

class Paragraph
{
	vector<Line*> Ls;
public:
	// **********CONSTRUCTORS********
	Paragraph();
	// ***************DESTRUCTOR***********
	~Paragraph();

	// *************FUNCTIONS**************

	void PrintLs()const;
	Line* getline(int Li);
	int GetSize()const;
	void NewLine(int ind);
	void InsertCAt(char c, Cursor& cur);
	void HandleEnter(Cursor& cur);
	void HandleBackspace(Cursor& cur);
	//void DeleteCAt(Cursor& cur);
	void Indent(Cursor& cur);
	void UnIndent(Cursor& cur);
	void DeleteLine(int ind);
	void DeleteAtCurrentPos(Cursor& cur);
	void PasteLine(Line* L, int ind);
	void DeleteFromCursorToEnd(Cursor cur);
	int FindNextWordStart(Cursor& cur);
	int FindPrevWordStart(Cursor& cur);
};
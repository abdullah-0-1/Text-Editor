#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "Sections.h"

using namespace std;
class Chapters
{
	vector<Sections*> Ss;
public:
	// **********CONSTRUCTORS********
	Chapters();

	// ***************DESTRUCTOR***********
	~Chapters();

	// *************FUNCTIONS**************
	Sections* GetSection(int Si);
	void AddSection(Sections* S);
	void InsertSectionAt(Sections* sec, int index);
	void PrintSs(int Srow) const;
	int GetSize() const;
	void NewSection(int ind);
	void DeleteSection(int ind);
	void InsertCAt(char c, Cursor& cur);
	void HandleEnter(Cursor& cur);
	void HandleBackspace(Cursor& cur);
	void Indent(Cursor& cur);
	void UnIndent(Cursor& cur);
	void DeleteAtCurrentPos(Cursor& cur);
	void PasteLine(Line* L, Cursor& cur);
	void DeleteFromCursorToEnd(Cursor& cur);
	int ComputeRow(int ChapInd, int ParaInd, int LineInd);
};

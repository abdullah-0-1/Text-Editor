#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "Paragraph.h"
using namespace std;




class Sections
{
    vector<Paragraph*> Ps;

public:

    // **********CONSTRUCTORS********
    Sections();

    // ***************DESTRUCTOR***********
    ~Sections();

    // *************FUNCTIONS**************
    Paragraph* GetParagraph(int Pi);
    void AddParagraph(Paragraph* P);
    void InsertParagraphAt(Paragraph* para, int index);
    void PrintPs(int) const;
    int GetSize() const;
    void NewParagraph(int ind);
    void DeleteParagraph(int ind);
    void InsertCAt(char c, Cursor& cur);
    void HandleEnter(Cursor& cur);
    void HandleBackspace(Cursor& cur);
    void Indent(Cursor& cur);
    void UnIndent(Cursor& cur);
    void DeleteAtCurrentPos(Cursor& cur);
    void PasteLine(Line* L, Cursor& cur);
    void DeleteFromCursorToEnd(Cursor& cur);
    int ComputeRow(int ParaInd, int LineInd);
    void FindNextWordStart(Cursor& cur);
    void FindPrevWordStart(Cursor& cur);
};


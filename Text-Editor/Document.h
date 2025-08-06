#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "Chapters.h"
#include<fstream>
using namespace std;

class Document
{
    vector<Chapters*> Cs;

public:
    // **********CONSTRUCTORS********
    Document();

    // ***************DESTRUCTOR***********
    ~Document();

    // *************FUNCTIONS**************
    Chapters* GetChapter(int Ci);
    void AddChapter(Chapters* C);
    void InsertChapterAt(Chapters* chap, int index);
    void PrintCs(int Srow) const;
    int GetSize() const;
    void NewChapter(int ind);
    void DeleteChapter(int ind);
    void InsertCAt(char c, Cursor& cur);
    void HandleEnter(Cursor& cur);
    void HandleBackspace(Cursor& cur);
    void Indent(Cursor& cur);
    void UnIndent(Cursor& cur);
    void DeleteAtCurrentPos(Cursor& cur);
    void PasteLine(Line* L, Cursor& cur);
    void DeleteFromCursorToEnd(Cursor& cur);
    int ComputeRow(int ChapInd, int SecInd, int ParaInd, int LineInd);
    void SaveToFile(const char* filename);
};

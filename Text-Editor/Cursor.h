#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>



class Cursor
{
public:
    int ChapInd;
    int PageInd;
    int ParaInd;
    int cr, cc;
    int LineInd;
    int CharInd;



    Cursor()
    {
        ChapInd = 0;
        PageInd = 0;
        ParaInd = 0;
        cr = 0, cc = 0;
        LineInd = 0;
        CharInd = 0;
    }

    Cursor(int chapter, int page, int para, int line, int charPos)
    {
        ChapInd = chapter;
        PageInd = page;
        ParaInd = para;
        LineInd = line;
        CharInd = charPos;
    }

    
};
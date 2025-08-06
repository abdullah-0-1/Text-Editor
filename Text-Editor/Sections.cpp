#include "Sections.h"

void gotoRowCol2(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

// **********CONSTRUCTORS********
Sections::Sections()
{
    Ps.push_back(new Paragraph());
}

// ***************DESTRUCTOR***********
Sections::~Sections()
{
    for (auto p : Ps)
    {
        delete p;
    }
    Ps.clear();
}

// *************FUNCTIONS**************

Paragraph* Sections::GetParagraph(int Pi)
{
    return (Pi >= 0 && Pi < Ps.size()) ? Ps[Pi] : nullptr;
}

void Sections::AddParagraph(Paragraph* P)
{
    Ps.push_back(P);
}

void Sections::InsertParagraphAt(Paragraph* para, int index)
{
    if (index >= 0 && index <= Ps.size())
    {
        Ps.insert(Ps.begin() + index, para);
    }
}


void Sections::PrintPs(int Srow) const
{
    int CurrRow = 0;
    for (int i = 0; i < Ps.size(); i++)
    {
        for (int j = 0; j < Ps[i]->GetSize(); j++)
        {
            if (CurrRow >= Srow)
            {
                gotoRowCol2(CurrRow - Srow, 0);
                Ps[i]->getline(j)->PrintCs();
            }
            CurrRow++;
        }
        if (CurrRow >= Srow)
        {
            cout << endl;
        }
        CurrRow++;
    }
}


int Sections::GetSize() const
{
    return Ps.size();
}

void Sections::NewParagraph(int ind)
{
    InsertParagraphAt(new Paragraph(), ind);
}

void Sections::DeleteParagraph(int ind)
{
    if (ind >= 0 && ind < Ps.size())
    {
        delete Ps[ind]; Ps.erase(Ps.begin() + ind);
    }
}

void Sections::InsertCAt(char c, Cursor& cur)
{
    GetParagraph(cur.ParaInd)->InsertCAt(c, cur);
}

void Sections::HandleEnter(Cursor& cur)
{
    Paragraph* CP = GetParagraph(cur.ParaInd);
    Line* CL = CP->getline(cur.LineInd);
    if (CL->GetSize() == 0)
    {
        InsertParagraphAt(new Paragraph(), cur.ParaInd + 1);
        cur.ParaInd++;
        cur.LineInd = 0;
        cur.cc = 0;
        cur.cr = ComputeRow(cur.ParaInd, cur.LineInd);
    }
    else
    {
        CP->HandleEnter(cur);
    }
}

void Sections::HandleBackspace(Cursor& cur)
{
    GetParagraph(cur.ParaInd)->HandleBackspace(cur);
}

void Sections::Indent(Cursor& cur)
{
    GetParagraph(cur.ParaInd)->Indent(cur);
}

void Sections::UnIndent(Cursor& cur)
{
    GetParagraph(cur.ParaInd)->UnIndent(cur);
}

void Sections::DeleteAtCurrentPos(Cursor& cur)
{
    GetParagraph(cur.ParaInd)->DeleteAtCurrentPos(cur);
}

void Sections::PasteLine(Line* L, Cursor& cur)
{
    GetParagraph(cur.ParaInd)->PasteLine(L, cur.LineInd + 1);
}

void Sections::DeleteFromCursorToEnd(Cursor& cur)
{
    GetParagraph(cur.ParaInd)->DeleteFromCursorToEnd(cur);
}

int Sections::ComputeRow(int ParaInd, int LineInd)
{
    int row = 0;
    for (int i = 0; i < ParaInd; i++)
    {
        row += Ps[i]->GetSize() + 1;
    }
    row += LineInd;
    return row;
}



void Sections::FindNextWordStart(Cursor& cur)
{
    int pos = GetParagraph(cur.ParaInd)->FindNextWordStart(cur);
    if (pos == -1)
    {
        return;
    }
    else
    {
        cur.cc = pos;
    }

}

void Sections::FindPrevWordStart(Cursor& cur)
{
    int pos = GetParagraph(cur.ParaInd)->FindPrevWordStart(cur);
    if (pos == -1)
    {
        return;
    }
    else
    {
        cur.cc = pos;
    }
}



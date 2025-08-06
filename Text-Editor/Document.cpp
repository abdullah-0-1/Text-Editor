#include "Document.h"

// **********CONSTRUCTORS********
Document::Document()
{
    Cs.push_back(new Chapters());
}

// ***************DESTRUCTOR***********
Document::~Document()
{
    for (auto chapter : Cs)
    {
        delete chapter;
    }
    Cs.clear();
}

// *************FUNCTIONS**************
Chapters* Document::GetChapter(int Ci)
{
    return (Ci >= 0 && Ci < Cs.size()) ? Cs[Ci] : nullptr;
}

void Document::AddChapter(Chapters* C)
{
    Cs.push_back(C);
}

void Document::InsertChapterAt(Chapters* chap, int index)
{
    if (index >= 0 && index <= Cs.size())
    {
        Cs.insert(Cs.begin() + index, chap);
    }
}

void Document::PrintCs(int Srow) const
{
    int CR = 0;
    for (int i = 0; i < Cs.size(); i++)
    {
        int ChapRows = 0;
        for (int j = 0; j < Cs[i]->GetSize(); j++)
        {
            for (int k = 0; k < Cs[i]->GetSection(j)->GetSize(); k++)
            {
                ChapRows += Cs[i]->GetSection(j)->GetParagraph(k)->GetSize() + 1;
            }
        }
        if (CR + ChapRows > Srow)
        {
            Cs[i]->PrintSs(Srow - CR);
        }
        CR += ChapRows;
    }
}

int Document::GetSize() const
{
    return Cs.size();
}

void Document::NewChapter(int ind)
{
    InsertChapterAt(new Chapters(), ind);
}

void Document::DeleteChapter(int ind)
{
    if (ind >= 0 && ind < Cs.size())
    {
        delete Cs[ind];
        Cs.erase(Cs.begin() + ind);
    }
}

void Document::InsertCAt(char c, Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->InsertCAt(c, cur);
    }
}

void Document::HandleEnter(Cursor& cur)
{
    Chapters* CC = GetChapter(cur.ChapInd);
    if (!CC)
    {
        return;
    }

    Sections* CS = CC->GetSection(cur.PageInd);
    if (!CS)
    {
        return;
    }

    Paragraph* CP = CS->GetParagraph(cur.ParaInd);
    if (!CP)
    {
        return;
    }

    Line* CL = CP->getline(cur.LineInd);
    if (!CL)
    {
        return;
    }

    if (CL->GetSize() == 0)
    {
        CS->HandleEnter(cur);
        cur.cr = ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
        return;
    }

    bool StartWHash = false;
    if (CL->GetSize() > 0 && CL->GetCharAt(0) == '#')
    {
        StartWHash = true;
    }

    CP->HandleEnter(cur);

    if (StartWHash || (cur.LineInd > 0 && CP->getline(cur.LineInd)->GetSize() > 0 &&
        CP->getline(cur.LineInd)->GetCharAt(0) == '#'))
    {
        Chapters* NewC = new Chapters();
        Sections* NewS = new Sections();
        Paragraph* NewP = new Paragraph();

        Line* lineToMove = CP->getline(cur.LineInd);
        Line* NL = new Line(*lineToMove);

        NewP->DeleteLine(0);
        NewP->PasteLine(NL, 0);
        delete NL;

        CP->DeleteLine(cur.LineInd);

        NewS->DeleteParagraph(0);
        NewS->AddParagraph(NewP);

        NewC->DeleteSection(0);
        NewC->AddSection(NewS);

        InsertChapterAt(NewC, cur.ChapInd + 1);

        cur.ChapInd++;
        cur.PageInd = 0;
        cur.ParaInd = 0;
        cur.LineInd = 0;
        cur.cc = 0;
        cur.cr = ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
    }
}

void Document::HandleBackspace(Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->HandleBackspace(cur);
    }
}

void Document::Indent(Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->Indent(cur);
    }
}

void Document::UnIndent(Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->UnIndent(cur);
    }
}

void Document::DeleteAtCurrentPos(Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->DeleteAtCurrentPos(cur);
    }
}

void Document::PasteLine(Line* L, Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->PasteLine(L, cur);
    }
}

void Document::DeleteFromCursorToEnd(Cursor& cur)
{
    if (cur.ChapInd >= 0 && cur.ChapInd < Cs.size())
    {
        Cs[cur.ChapInd]->DeleteFromCursorToEnd(cur);
    }
}

int Document::ComputeRow(int ChapInd, int SecInd, int ParaInd, int LineInd)
{
    int row = 0;
    for (int i = 0; i < ChapInd; i++)
    {
        for (int j = 0; j < Cs[i]->GetSize(); j++)
        {
            for (int k = 0; k < Cs[i]->GetSection(j)->GetSize(); k++)
            {
                row += Cs[i]->GetSection(j)->GetParagraph(k)->GetSize() + 1;
            }
        }
    }
    if (ChapInd >= 0 && ChapInd < Cs.size())
    {
        row += Cs[ChapInd]->ComputeRow(SecInd, ParaInd, LineInd);
    }
    return row;
}

void Document::SaveToFile(const char* filename)
{
    ofstream wtr(filename);
    if (!wtr.is_open())
    {
        cout << "Failed to open file for writing." << endl;
        return;
    }
    for (int i = 0; i < Cs.size(); i++)
    {
        Chapters* chap = Cs[i];
        for (int j = 0; j < chap->GetSize(); j++)
        {
            Sections* sec = chap->GetSection(j);
            for (int k = 0; k < sec->GetSize(); k++)
            {
                Paragraph* para = sec->GetParagraph(k);
                for (int m = 0; m < para->GetSize(); m++)
                {
                    Line* line = para->getline(m);
                    for (int n = 0; n < line->GetSize(); n++)
                    {
                        char c = line->GetCharAt(n);
                        if (c == ' ')
                        {
                            wtr << '*';
                        }
                        else
                        {
                            wtr << c;
                        }
                    }
                    wtr << endl;
                }
            }
        }
    }
    wtr.close();
}
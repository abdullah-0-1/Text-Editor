#include "Chapters.h"

// **********CONSTRUCTORS********

Chapters::Chapters()
{
	Ss.push_back(new Sections());
}

// ***************DESTRUCTOR***********

Chapters::~Chapters()
{
	for (auto s : Ss)
	{
		delete s;
	}
	Ss.clear();
}

// *************FUNCTIONS**************

Sections* Chapters::GetSection(int Si)
{
	return (Si >= 0 && Si < Ss.size()) ? Ss[Si] : nullptr;
}

void Chapters::AddSection(Sections* S)
{
	Ss.push_back(S);
}

void Chapters::InsertSectionAt(Sections* sec, int index)
{
	if (index >= 0 && index <= Ss.size())
	{
		Ss.insert(Ss.begin() + index, sec);
	}
}
void Chapters::PrintSs(int Srow) const
{
	int CR = 0;
	for (int i = 0; i < Ss.size(); i++)
	{
		int SecRows = 0;
		for (int j = 0; j < Ss[i]->GetSize(); j++)
		{
			SecRows += Ss[i]->GetParagraph(j)->GetSize() + 1;
		}
		if (CR + SecRows > Srow)
		{
			Ss[i]->PrintPs(Srow - CR);
		}
		CR += SecRows;
	}
}

int Chapters::GetSize() const
{
	return Ss.size();
}

void Chapters::NewSection(int ind)
{
	InsertSectionAt(new Sections(), ind);
}

void Chapters::DeleteSection(int ind)
{
	if (ind >= 0 && ind < Ss.size())
	{
		delete Ss[ind];
		Ss.erase(Ss.begin() + ind);
	}
}

void Chapters::InsertCAt(char c, Cursor& cur)
{
	GetSection(cur.PageInd)->InsertCAt(c, cur);
}

void Chapters::HandleEnter(Cursor& cur)
{
	Sections* CS = GetSection(cur.PageInd);
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
		cur.cr = ComputeRow(cur.ChapInd, cur.ParaInd, cur.LineInd);
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

		InsertSectionAt(NewS, cur.ChapInd + 1);

		cur.ChapInd++;
		cur.ParaInd = 0;
		cur.LineInd = 0;
		cur.cc = 0;
		cur.cr = ComputeRow(cur.ChapInd, cur.ParaInd, cur.LineInd);
	}
}

void Chapters::HandleBackspace(Cursor& cur)
{
	GetSection(cur.ChapInd)->HandleBackspace(cur);
}


void Chapters::Indent(Cursor& cur)
{
	GetSection(cur.ChapInd)->Indent(cur);
}

void Chapters::UnIndent(Cursor& cur)
{
	GetSection(cur.ChapInd)->UnIndent(cur);
}

void Chapters::DeleteAtCurrentPos(Cursor& cur)
{
	GetSection(cur.ChapInd)->DeleteAtCurrentPos(cur);
}

void Chapters::PasteLine(Line* L, Cursor& cur)
{
	GetSection(cur.ChapInd)->PasteLine(L, cur);
}

void Chapters::DeleteFromCursorToEnd(Cursor& cur)
{
	GetSection(cur.ChapInd)->DeleteFromCursorToEnd(cur);
}

int Chapters::ComputeRow(int ChapInd, int ParaInd, int LineInd)
{
	int row = 0;
	for (int i = 0; i < ChapInd; i++)
	{
		for (int j = 0; j < Ss[i]->GetSize(); j++)
		{
			row += Ss[i]->GetParagraph(j)->GetSize() + 1;
		}
	}
	row += GetSection(ChapInd)->ComputeRow(ParaInd, LineInd);
	return row;
}
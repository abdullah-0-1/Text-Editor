#include "Paragraph.h"


// **********CONSTRUCTORS********

Paragraph::Paragraph()
{
	Ls.push_back(new Line());
}

// ***************DESTRUCTOR***********

Paragraph::~Paragraph()
{
	for (auto line : Ls)
	{
		delete line;
	}
	Ls.clear();
}

// *************FUNCTIONS**************

Line* Paragraph::getline(int Li)
{
	return(this->Ls[Li]);
}
int Paragraph::GetSize()const
{
	return Ls.size();
}

void Paragraph::PrintLs()const
{
	for (int i = 0; i < Ls.size(); i++)
	{
		Ls[i]->PrintCs();
		cout << endl;
	}
}

void Paragraph::HandleEnter(Cursor& cur)
{
	if (cur.LineInd < 0 || cur.LineInd >= Ls.size())
	{
		return;
	}
	Line* current = Ls[cur.LineInd];
	Line* right = current->Split(cur.cc);
	Ls.insert(Ls.begin() + cur.LineInd + 1, right);
	cur.LineInd++, cur.cr++;
	cur.cc = 0;
}

void Paragraph::Indent(Cursor& cur)
{
	getline(cur.LineInd)->Indent(cur);
}
void Paragraph::UnIndent(Cursor& cur)
{
	getline(cur.LineInd)->UnIndent(cur);
}

void Paragraph::NewLine(int ind)
{
	Line* NL = new Line();
	this->Ls.insert(Ls.begin() + ind, NL);
}

void Paragraph::InsertCAt(char c, Cursor& cur)
{
	getline(cur.LineInd)->InsertCAt(c, cur.cc);
	cur.cc++;
}

void Paragraph::DeleteLine(int ind)
{

	if (ind >= 0 && ind < Ls.size())
	{
		delete Ls[ind];
		Ls.erase(Ls.begin() + ind);
	}
}

void Paragraph::DeleteAtCurrentPos(Cursor& cur)
{
	Line* CurrL = getline(cur.LineInd);
	if (CurrL != nullptr)
	{
		int Lsize = CurrL->GetSize();
		if (cur.cc < Lsize && !CurrL->IsDeli(cur.cc))
		{
			if (cur.cc == 0 && Lsize == 1)
			{
				if (cur.cr != 0)
				{
					DeleteLine(cur.LineInd);
					cur.LineInd--;
					cur.cr--;
				}
			}
			CurrL->DeleteCAt(cur.cc);
		}
	}

}

void Paragraph::HandleBackspace(Cursor& cur)
{
	Line* CL = getline(cur.LineInd);
	if (CL != nullptr)
	{
		if (cur.cc > 0)
		{
			CL->DeleteCAt(cur.cc - 1);
			cur.cc--;
		}
		else if (cur.LineInd > 0)
		{
			Line* PL = getline(cur.LineInd - 1);
			if (PL != nullptr)
			{
				int Psize = PL->GetSize();
				int Csize = CL->GetSize();
				if (Psize + Csize <= CL->MaxCs)
				{
					PL->MergeLines(*CL);
					DeleteLine(cur.LineInd);
					cur.LineInd--;
					cur.cr--;
					cur.cc = Psize;
				}
			}
		}
	}
}

void Paragraph::PasteLine(Line* L, int ind)
{
	Line* NL = new Line(*L);
	Ls.insert(Ls.begin() + ind, NL);
}

void Paragraph::DeleteFromCursorToEnd(Cursor cur)
{
	getline(cur.LineInd)->DeleteFromCursorToEnd(cur.cc);
}


int Paragraph::FindNextWordStart(Cursor& cur)
{
	int pos = getline(cur.LineInd)->FindNextWordStart(cur.cc);
	return pos;
}

int Paragraph::FindPrevWordStart(Cursor& cur)
{
	int pos = getline(cur.LineInd)->FindPrevWordStart(cur.cc);
	return pos;
}

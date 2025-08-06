#include "line.h"

// ***************CONSTRUCTORS***************
Line::Line()
{
	this->Cs = new char[1] {};
	this->size = 0;
}

Line::Line(const Line& L)
{
	DeepCopy(L);
}

Line::Line(const char* str, int l)
{
	this->Cs = nullptr;
	this->size = l;
	if (str && l > 0)
	{
		this->Cs = new char[this->size + 1] {};
		for (int i = 0; i < this->size; i++)
		{
			this->Cs[i] = str[i];
		}
	}
}

Line::Line(const char* str)
{
	this->Cs = nullptr;
	this->size = 0;
	if (str)
	{
		while (str[this->size] != '\0')
		{
			this->size++;
		}
		this->Cs = new char[this->size + 1] {};
		for (int i = 0; i < this->size; i++)
		{
			this->Cs[i] = str[i];
		}
	}
}

// **************DESTRUCTOR**********

Line::~Line()
{
	Clear();
}

// ******************FUNCTIONS***********

void Line::DeepCopy(const Line& L)
{
	this->Cs = new char[L.size + 1] {};
	this->size = L.size;
	for (int i = 0; i < L.size; i++)
	{
		this->Cs[i] = L.Cs[i];
	}
}

void Line::Clear()
{
	delete[] this->Cs;
	this->Cs = nullptr;
	this->size = 0;
}

void Line::InsertCAt(char c, int ind)
{
	char* NA = new char[this->size + 2] {};
	for (int i = 0; i < ind; i++)
	{
		NA[i] = this->Cs[i];
	}
	NA[ind] = c;
	for (int i = ind; i < this->size; i++)
	{
		NA[i + 1] = this->Cs[i];
	}
	delete[] this->Cs;
	this->Cs = NA;
	this->size++;
}

void Line::Append(char c)
{
	InsertCAt(c, this->size);
}

void Line::DeleteCAt(int ind)
{
	if (ind >= this->size)
	{
		return;
	}
	char* NA = new char[this->size] {};
	for (int i = 0; i < ind; i++)
	{
		NA[i] = this->Cs[i];
	}
	for (int i = ind + 1; i < this->size; i++)
	{
		NA[i - 1] = this->Cs[i];
	}
	NA[this->size - 1] = '\0';
	delete[] this->Cs;
	this->Cs = NA;
	this->size--;
}

int Line::GetSize()
{
	return this->size;
}

char Line::ToUpper(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return c - 32;
	}
	return c;
}

char* Line::ToUpper(char* C, int l)
{
	char* U = new char[l + 1] {};
	for (int i = 0; i < l; i++)
	{
		U[i] = ToUpper(C[i]);
	}
	return U;
}

char Line::ToLower(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return c + 32;
	}
	return c;
}

char* Line::ToLower(char* C, int l)
{
	char* L = new char[l + 1] {};
	for (int i = 0; i < l; i++)
	{
		L[i] = ToLower(C[i]);
	}
	return L;
}

vector<int> Line::Substr(char* sub, int l)
{
	vector<int> Inds;
	for (int i = 0; i < this->size - l; i++)
	{
		int c = 0;
		for (int j = 0; j < l; j++)
		{
			if (sub[j] != this->Cs[i + j])
			{
				break;
			}
			else
			{
				c++;
			}
		}
		if (c == l)
		{
			Inds.push_back(i);
		}
	}
	return Inds;
}

void Line::PrintCs()const
{
	for (int i = 0; i < this->size; i++)
	{
		cout << this->Cs[i];
	}
}

void Line::Indent(Cursor& cur)
{
	for (int i = 0; i < 4; i++)
	{
		InsertCAt(' ', cur.cc);
		cur.cc++;
	}
}

void Line::UnIndent(Cursor& cur)
{
	int c = cur.cc - 1;
	for (int i = 0; i < 4; i++)
	{
		if (this->Cs[c] != ' ')
		{
			return;
		}
		c--;
	}
	c = cur.cc - 1;
	for (int i = 0; i < 4; i++)
	{
		DeleteCAt(c);
		c--;
	}
	cur.cc -= 4;
}

bool Line::IsDeli(const int& cur)const
{
	if (cur < 0 || cur >= this->size)
	{
		return false;
	}

	char ch = this->Cs[cur];
	return (ch == ' ' || ch == '\t' || ch == '\n');
}

void Line::Trim(int Nsize)
{
	if (Nsize< 0 || Nsize> this->size)
	{
		return;
	}
	char* NA = new char[Nsize + 1] {};
	for (int i = 0; i < Nsize; i++)
	{
		NA[i] = this->Cs[i];
	}
	NA[Nsize] = '\0';
	delete[] this->Cs;
	this->Cs = NA;
	size = Nsize;
}

Line* Line::Split(int p)
{
	if (p < 0 || p > this->size)
	{
		return nullptr;
	}
	Line* right = new Line(this->Cs + p, this->size - p);
	Trim(p);
	return right;
}

void Line::MergeLines(const Line& L)
{
	int NewSize = this->size + L.size;
	char* NA = new char[NewSize + 1] {};
	for (int i = 0; i < this->size; i++)
	{
		NA[i] = this->Cs[i];
	}
	for (int i = 0; i < L.size; i++)
	{
		NA[this->size + i] = L.Cs[i];
	}
	delete[] Cs;
	Cs = NA;
	size = NewSize;
}

char Line::GetCharAt(int ind)
{
	return this->Cs[ind];
}

void Line::ToggleCase(int ind)
{
	char c = ' ';
	if (!(GetCharAt(ind) >= 'a' && GetCharAt(ind) <= 'z') && !(GetCharAt(ind) >= 'A' && GetCharAt(ind) <= 'Z'))
	{
		return;
	}
	if (GetCharAt(ind) >= 'a' && GetCharAt(ind) <= 'z')
	{
		c = ToUpper(GetCharAt(ind));
	}
	else if (GetCharAt(ind) >= 'A' && GetCharAt(ind) <= 'Z')
	{
		c = ToLower(GetCharAt(ind));
	}
	this->Cs[ind] = c;
}

void Line::DeleteFromCursorToEnd(int ind)
{
	if (ind >= 0 && ind <= this->size)
	{
		Trim(ind);
	}
}

int Line::FindNextWordStart(int startPos) const
{
	for (int pos = startPos + 1; pos < size; pos++)
	{
		if (!IsDeli(pos) && (pos == 0 || IsDeli(pos - 1)))
		{
			return pos;
		}
	}
	return -1;
}

int Line::FindPrevWordStart(int startPos) const
{
	for (int pos = startPos - 1; pos >= 0; pos--)
	{
		if (!IsDeli(pos) && (pos == 0 || IsDeli(pos - 1)))
		{
			return pos;
		}
	}
	return -1;
}
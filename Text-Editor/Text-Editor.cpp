#include "Document.h"
#include"Cursor.h"
#include<cstring>
using namespace std;

enum Mode
{
    NORMAL,
    INSERT,
    COMMAND_LINE
};

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

void Editing(Document& D, Mode& M, Cursor& cur)
{
    int key, key2;
    char sym;
    Line ClipBoard;
    M = NORMAL;
    const int Height = 25;
    int Scroll = 0;

    while (true)
    {
        if (cur.cr >= Height)
        {
            Scroll = cur.cr - Height + 1;
        }
        else
        {
            Scroll = 0;
        }

        if (M == NORMAL)
        {
            if (_kbhit())
            {
                key = _getch();
                switch (key)
                {
                case 4: // Ctrl+d (scroll down half page)
                {
                    Scroll += Height / 2;
                    int MaxChapInd = D.GetSize() - 1;
                    int MaxSecInd = D.GetChapter(MaxChapInd)->GetSize() - 1;
                    int MaxParaInd = D.GetChapter(MaxChapInd)->GetSection(MaxSecInd)->GetSize() - 1;
                    int MaxLineInd = D.GetChapter(MaxChapInd)->GetSection(MaxSecInd)->GetParagraph(MaxParaInd)->GetSize() - 1;
                    int MaxScroll = D.ComputeRow(MaxChapInd, MaxSecInd, MaxParaInd, MaxLineInd);

                    if (Scroll > MaxScroll - Height + 1)
                    {
                        Scroll = max(0, MaxScroll - Height + 1);
                    }
                }
                break;

                case 21: // Ctrl+u (scroll up half page)
                    Scroll -= Height / 2;
                    if (Scroll < 0)
                    {
                        Scroll = 0;
                    }
                    break;
                case 103: //(gg) Move to begining of file
                    key2 = _getch();
                    if (key2 == 103)
                    {
                        cur.ChapInd = 0;
                        cur.PageInd = 0;
                        cur.ParaInd = 0;
                        cur.LineInd = 0;
                        cur.cc = 0;
                        cur.cr = 0;
                    }
                    break;

                case 71: //(G) Move to the end of file
                    cur.ChapInd = D.GetSize() - 1;
                    cur.PageInd = D.GetChapter(cur.ChapInd)->GetSize() - 1;
                    cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                    cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                    cur.cc = 0;
                    cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    break;

                case 107: // (k) up
                    if (cur.LineInd > 0)
                    {
                        cur.LineInd--;
                        cur.cr--;
                    }
                    else if (cur.ParaInd > 0)
                    {
                        cur.ParaInd--;
                        cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    }
                    else if (cur.PageInd > 0)
                    {
                        cur.PageInd--;
                        cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                        cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    }
                    else if (cur.ChapInd > 0)
                    {
                        cur.ChapInd--;
                        cur.PageInd = D.GetChapter(cur.ChapInd)->GetSize() - 1;
                        cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                        cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    }
                    {
                        int lineSize = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                        if (cur.cc > lineSize)
                        {
                            cur.cc = lineSize;
                        }
                    }
                    break;
                case 106: // (j) down
                    if (cur.LineInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1)
                    {
                        cur.LineInd++;
                        cur.cr++;
                    }
                    else if (cur.ParaInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1)
                    {
                        cur.ParaInd++;
                        cur.LineInd = 0;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    }
                    else if (cur.PageInd < D.GetChapter(cur.ChapInd)->GetSize() - 1)
                    {
                        cur.PageInd++;
                        cur.ParaInd = 0;
                        cur.LineInd = 0;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    }
                    else if (cur.ChapInd < D.GetSize() - 1)
                    {
                        cur.ChapInd++;
                        cur.PageInd = 0;
                        cur.ParaInd = 0;
                        cur.LineInd = 0;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                    }
                    {
                        int lineSize = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                        if (cur.cc > lineSize)
                        {
                            cur.cc = lineSize;
                        }
                    }
                    break;
                case 108: // (l) right
                    if (cur.cc < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize())
                    {
                        cur.cc++;
                    }
                    else if (cur.LineInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1)
                    {
                        cur.LineInd++;
                        cur.cr++;
                        cur.cc = 0;
                    }
                    else if (cur.ParaInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1)
                    {
                        cur.ParaInd++;
                        cur.LineInd = 0;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        cur.cc = 0;
                    }
                    else if (cur.PageInd < D.GetChapter(cur.ChapInd)->GetSize() - 1)
                    {
                        cur.PageInd++;
                        cur.ParaInd = 0;
                        cur.LineInd = 0;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        cur.cc = 0;
                    }
                    else if (cur.ChapInd < D.GetSize() - 1)
                    {
                        cur.ChapInd++;
                        cur.PageInd = 0;
                        cur.ParaInd = 0;
                        cur.LineInd = 0;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        cur.cc = 0;
                    }
                    break;
                case 104: // (h) left
                    if (cur.cc > 0)
                    {
                        cur.cc--;
                    }
                    else if (cur.LineInd > 0)
                    {
                        cur.LineInd--;
                        cur.cr--;
                        cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                    }
                    else if (cur.ParaInd > 0)
                    {
                        cur.ParaInd--;
                        cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                    }
                    else if (cur.PageInd > 0)
                    {
                        cur.PageInd--;
                        cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                        cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                    }
                    else if (cur.ChapInd > 0)
                    {
                        cur.ChapInd--;
                        cur.PageInd = D.GetChapter(cur.ChapInd)->GetSize() - 1;
                        cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                        cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                        cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                    }
                    break;
                case 105: // (i) insert mode
                    M = INSERT;
                    break;
                case 120: // (x) delete at current pos
                    D.DeleteAtCurrentPos(cur); // Updated from C to D
                    break;
                case 100: // (d) check for dd
                    key2 = _getch();
                    if (key2 == 100) // (dd) delete current line
                    {
                        Paragraph* currPara = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd);
                        if (currPara->getline(cur.LineInd)->GetSize() > 0)
                        {
                            currPara->DeleteLine(cur.LineInd);
                            if (cur.LineInd > 0)
                            {
                                cur.LineInd--;
                                cur.cr--;
                            }
                            else if (cur.ParaInd > 0)
                            {
                                cur.ParaInd--;
                                cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                                cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            }
                            else if (cur.PageInd > 0)
                            {
                                cur.PageInd--;
                                cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                                cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                                cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            }
                            else if (cur.ChapInd > 0)
                            {
                                cur.ChapInd--;
                                cur.PageInd = D.GetChapter(cur.ChapInd)->GetSize() - 1;
                                cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                                cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                                cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            }
                            cur.cc = 0;
                        }
                    }
                    break;
                case 48: // (0) move to beginning of line
                    cur.cc = 0;
                    break;
                case 36: // ($) move to end of line
                    cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                    break;
                case 121: // (y) check for yy
                    key2 = _getch();
                    if (key2 == 121) // (yy) yank current line
                    {
                        ClipBoard.DeepCopy(*(D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)));
                    }
                    break;
                case 112: // (p) paste after line
                    D.PasteLine(&ClipBoard, cur);
                    break;
                case 80: // (P) paste before line
                    D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->PasteLine(&ClipBoard, cur.LineInd);
                    break;
                case 126: // (~) toggle case
                    D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->ToggleCase(cur.cc);
                    break;
                case 68: // (D) delete from cursor to end
                    D.DeleteFromCursorToEnd(cur);
                    break;
                case 119: // (w) move to next word
                {
                    int pos = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->FindNextWordStart(cur);
                    if (pos != -1)
                    {
                        cur.cc = pos;
                    }
                }
                break;
                case 98: // (b) move to previous word
                {
                    int pos = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->FindPrevWordStart(cur);
                    if (pos != -1)
                    {
                        cur.cc = pos;
                    }
                }
                break;
                case 58: //(:) Command Line
                    M = COMMAND_LINE;
                    break;
                default:
                    M = NORMAL;
                    break;
                }

                system("cls");
                D.PrintCs(Scroll);
                gotoRowCol(cur.cr - Scroll, cur.cc);
            }
        }
        else if (M == INSERT)
        {
            if (_kbhit())
            {
                key = _getch();
                if (key == 224)
                {
                    key = _getch();
                    switch (key)
                    {
                    case 72: // up
                        if (cur.LineInd > 0)
                        {
                            cur.LineInd--;
                            cur.cr--;
                        }
                        else if (cur.ParaInd > 0)
                        {
                            cur.ParaInd--;
                            cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        }
                        else if (cur.PageInd > 0)
                        {
                            cur.PageInd--;
                            cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                            cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        }
                        else if (cur.ChapInd > 0)
                        {
                            cur.ChapInd--;
                            cur.PageInd = D.GetChapter(cur.ChapInd)->GetSize() - 1;
                            cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                            cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        }
                        {
                            int lineSize = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                            if (cur.cc > lineSize)
                            {
                                cur.cc = lineSize;
                            }
                        }
                        break;
                    case 80: // down
                        if (cur.LineInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1)
                        {
                            cur.LineInd++;
                            cur.cr++;
                        }
                        else if (cur.ParaInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1)
                        {
                            cur.ParaInd++;
                            cur.LineInd = 0;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        }
                        else if (cur.PageInd < D.GetChapter(cur.ChapInd)->GetSize() - 1)
                        {
                            cur.PageInd++;
                            cur.ParaInd = 0;
                            cur.LineInd = 0;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        }
                        else if (cur.ChapInd < D.GetSize() - 1)
                        {
                            cur.ChapInd++;
                            cur.PageInd = 0;
                            cur.ParaInd = 0;
                            cur.LineInd = 0;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                        }
                        {
                            int lineSize = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                            if (cur.cc > lineSize)
                            {
                                cur.cc = lineSize;
                            }
                        }
                        break;
                    case 77: // right
                        if (cur.cc < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize())
                        {
                            cur.cc++;
                        }
                        else if (cur.LineInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1)
                        {
                            cur.LineInd++;
                            cur.cr++;
                            cur.cc = 0;
                        }
                        else if (cur.ParaInd < D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1)
                        {
                            cur.ParaInd++;
                            cur.LineInd = 0;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            cur.cc = 0;
                        }
                        else if (cur.PageInd < D.GetChapter(cur.ChapInd)->GetSize() - 1)
                        {
                            cur.PageInd++;
                            cur.ParaInd = 0;
                            cur.LineInd = 0;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            cur.cc = 0;
                        }
                        else if (cur.ChapInd < D.GetSize() - 1)
                        {
                            cur.ChapInd++;
                            cur.PageInd = 0;
                            cur.ParaInd = 0;
                            cur.LineInd = 0;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            cur.cc = 0;
                        }
                        break;
                    case 75: // left
                        if (cur.cc > 0)
                        {
                            cur.cc--;
                        }
                        else if (cur.LineInd > 0)
                        {
                            cur.LineInd--;
                            cur.cr--;
                            cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                        }
                        else if (cur.ParaInd > 0)
                        {
                            cur.ParaInd--;
                            cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                        }
                        else if (cur.PageInd > 0)
                        {
                            cur.PageInd--;
                            cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                            cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                        }
                        else if (cur.ChapInd > 0)
                        {
                            cur.ChapInd--;
                            cur.PageInd = D.GetChapter(cur.ChapInd)->GetSize() - 1;
                            cur.ParaInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetSize() - 1;
                            cur.LineInd = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->GetSize() - 1;
                            cur.cr = D.ComputeRow(cur.ChapInd, cur.PageInd, cur.ParaInd, cur.LineInd);
                            cur.cc = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd)->GetSize();
                        }
                        break;
                    case 83: // delete
                        D.DeleteAtCurrentPos(cur);
                        break;
                    }
                }
                else if (key == 13) // Enter
                {
                    D.HandleEnter(cur);
                }
                else if (GetKeyState(VK_SHIFT) & 0x8000 && key == 9) // Shift+Tab
                {
                    D.UnIndent(cur);
                }
                else if (key == 9) // Tab
                {
                    D.Indent(cur);
                }
                else if (key == 27) // Escape
                {
                    M = NORMAL;
                }
                else if (key == 8) // Backspace
                {
                    D.HandleBackspace(cur);
                }
                else
                {
                    sym = (char)key;
                    Line* CL = D.GetChapter(cur.ChapInd)->GetSection(cur.PageInd)->GetParagraph(cur.ParaInd)->getline(cur.LineInd);
                    if (CL->GetSize() >= CL->MaxCs)
                    {
                        D.HandleEnter(cur);
                        D.InsertCAt(sym, cur);
                    }
                    else
                    {
                        D.InsertCAt(sym, cur);
                    }
                }
                system("cls");
                D.PrintCs(Scroll);
                gotoRowCol(cur.cr - Scroll, cur.cc);
            }
        }
        else if (M == COMMAND_LINE)
        {
            gotoRowCol(24, 0); // Assuming screen height is 25, use row 24 for command
            cout << ":";
            char command[100] = { 0 };
            int idx = 0;
            while (true)
            {
                if (_kbhit())
                {
                    key = _getch();
                    if (key == 13) // Enter
                    {
                        command[idx] = '\0';
                        break;
                    }
                    else if (key == 27) // Esc
                    {
                        M = NORMAL;
                        break;
                    }
                    else if (key == 8) // Backspace
                    {
                        if (idx > 0)
                        {
                            idx--;
                            command[idx] = '\0';
                            gotoRowCol(24, idx + 1);
                            cout << ' ';
                            gotoRowCol(24, idx + 1);
                        }
                    }
                    else if (idx < 99)
                    {
                        command[idx] = (char)key;
                        cout << (char)key;
                        idx++;
                    }
                }
            }
            if (M == COMMAND_LINE)
            {
                if (strcmp(command, "w") == 0)
                {
                    D.SaveToFile("DOC.txt");
                }
                else if (strcmp(command, "q") == 0)
                {
                    exit(0);
                }
                else if (strcmp(command, "wq") == 0)
                {
                    D.SaveToFile("DOC.txt");
                    exit(0);
                }
                else if (strcmp(command, "q!") == 0)
                {
                    exit(0);
                }
                M = NORMAL;
            }
            gotoRowCol(24, 0);
            for (int i = 0; i < 20; i++)  cout << ' ';
            system("cls");
            D.PrintCs(Scroll);
            gotoRowCol(cur.cr - Scroll, cur.cc);
        }
    }
}

int main()
{
    Cursor cur;
    Document D;
    Mode M = NORMAL;
    Editing(D, M, cur);
    return 0;
}
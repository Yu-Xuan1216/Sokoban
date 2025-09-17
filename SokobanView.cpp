
// SokobanView.cpp: CSokobanView 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Sokoban.h"
#endif

#include "SokobanDoc.h"
#include "SokobanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <iostream>
using std::string;
using std::ifstream;
using std::istringstream;
using std::stringstream;
using std::vector;
using namespace std;

int level = 0;
enum Direction
{
    LEFT,
    UP,
    DOWN,
    RIGHT
} dir;

// CSokobanView

IMPLEMENT_DYNCREATE(CSokobanView, CView)

BEGIN_MESSAGE_MAP(CSokobanView, CView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CSokobanView 建構/解構

CSokobanView::CSokobanView() noexcept
{
	// TODO: 在此加入建構程式碼
    GameStart();
}

CSokobanView::~CSokobanView()
{
}

BOOL CSokobanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CSokobanView 繪圖

void CSokobanView::OnDraw(CDC* /*pDC*/)
{
	CSokobanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	WriteToScreen();
}


// CSokobanView 列印

BOOL CSokobanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CSokobanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CSokobanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}


// CSokobanView 診斷

#ifdef _DEBUG
void CSokobanView::AssertValid() const
{
	CView::AssertValid();
}

void CSokobanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSokobanDoc* CSokobanView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSokobanDoc)));
	return (CSokobanDoc*)m_pDocument;
}
#endif //_DEBUG


// CSokobanView 訊息處理常式


void CSokobanView::WriteToScreen()
{
    CClientDC aDC(this);
    CString s;
    

    MaxBlank(); // 查空地大小
    if (level < 151)
    {
        showMap();
        box = check_box();

        s.Format(L"Level: %d    ", level);
        aDC.TextOut(45 * maxblank, 10, s);
        s.Format(L"Destination: %d    ", D_count);
        aDC.TextOut(45 * maxblank, 40, s);
        s.Format(L"Arrival: %d   ", box);
        aDC.TextOut(45 * maxblank, 70, s);
        s.Format(L"Steps: %d   ", steps);
        aDC.TextOut(45 * maxblank, 100, s);
        s.Format(L"R: Restart");
        aDC.TextOut(45 * maxblank, 130, s);
        /*s.Format(L"N: Next level");
        aDC.TextOut(45 * maxblank, 160, s);*/

        if (box == D_count)
        {
            AfxMessageBox(L"Pass!\nPress SPACE to the next level.", MB_OK);
        }
    }

    

}


int CSokobanView::readMap(int n)
{
    // get file path
    stringstream stream;
    string filename = "./source/map";  
    stream  << n;
    if (n < 10)
        filename = filename + "00" + stream.str() +".txt";
    else if (n >= 10 && n < 100)
        filename = filename + "0" + stream.str() + ".txt";
    else
        filename = filename + stream.str() + ".txt";
    stream.clear();

    // read file
    ifstream ifs(filename);
    if (!ifs.is_open())
        AfxMessageBox(L"Map not find.", MB_OK);
    else
    {
        string s;
        int row = 0;
        while (getline(ifs, s))
        {
            for (int i = 0; i < (int)s.length(); i++)
            {
                switch (s[i])
                {
                case 'H':
                    break;
                case '0':
                    s[i] = ' ';
                    break;
                case ' ':
                    break;
                case 'B':
                    B_count++;
                    break;
                case 'C':
                {
                    B_count++;
                    D_count++;
                    break;
                }
                case 'D':
                    D_count++;
                    break;
                case 'W':
                {
                    s[i] = ' ';
                    W_x = i;
                    W_y = row;
                    W_count++;
                    break;
                }
                default:
                    invalid++;
                    break;
                }
            }
            row++;
            Map.push_back(s);
        }

    }
    return 1;
    
}


void CSokobanView::showMap()
{
    int y = 0, x = 0;
    char tmp;
    CClientDC aDC(this); 
    CBitmap worker_Bitmap, arrival_Bitmap, blank_Bitmap, box_Bitmap, dest_Bitmap, wall_Bitmap;
    worker_Bitmap.LoadBitmap(IDB_WORKER);
    arrival_Bitmap.LoadBitmap(IDB_ARRIVAL);
    blank_Bitmap.LoadBitmap(IDB_BLANK);
    dest_Bitmap.LoadBitmap(IDB_DESTINATION);
    wall_Bitmap.LoadBitmap(IDB_WALL);
    box_Bitmap.LoadBitmap(IDB_BOX);
    CDC memDC;
    memDC.CreateCompatibleDC(&aDC);

    // display
    for (int i = 0; i < (int)Map.size(); i++)
    {
        // 填滿方形空地
        for (int j = 0; j < maxblank+1 ; j++)
        {
            memDC.SelectObject(&blank_Bitmap);
            aDC.BitBlt(32 * x, 32 * y, 32, 32, &memDC, 0, 0, SRCCOPY);
            x++;
        }
        x = 0;

        // 填滿地圖元素
        for (int j = 0; j < (int)Map[i].length(); j++)
        {
            tmp = Map[i][j];
            if (tmp == ' ')
                memDC.SelectObject(&blank_Bitmap);
            if (tmp=='B')
                memDC.SelectObject(&box_Bitmap);
            if (tmp == 'C')
                memDC.SelectObject(&arrival_Bitmap);
            if (tmp == 'D')
                memDC.SelectObject(&dest_Bitmap);
            if (i == W_y && j == W_x)
                memDC.SelectObject(&worker_Bitmap);
            if(tmp=='H')
                memDC.SelectObject(&wall_Bitmap);

            aDC.BitBlt(32*x, 32*y, 32, 32, &memDC, 0, 0, SRCCOPY);
            x++;
        }
        x = 0;
        y++;
    }
}


// 空地大小
void CSokobanView::MaxBlank()
{
    maxblank = 0;
    for (int i = 0; i < (int)Map.size(); i++)
    {
        for (int j = 0; j < (int)Map[i].length(); j++)
        {
            if (j > maxblank)
                maxblank = j;
        }
    }
}


char CSokobanView::NextToWorker(int d)
{
    char next;
    switch (d)
    {
    case 0: // H: left
        next = Map[W_y][W_x - 1];
        break;
    case 1: // K: up
        next = Map[W_y - 1][W_x];
        break;
    case 2: // J: down
        next = Map[W_y + 1][W_x];
        break;
    case 3: // L: right
        next = Map[W_y][W_x + 1];
        break;
    }

    return next;
}


void CSokobanView::moveWorker(int d)
{
    switch (d)
    {
    case 0: // H: left
    {
        W_x--;
        steps++;
        break;
    }
    case 1: // K: up
    {
        W_y--;
        steps++;
        break;
    }
    case 2: // J: down
    {
        W_y++;
        steps++;
        break;
    }
    case 3: // L: right
    {
        W_x++;
        steps++;
        break;
    }
    }
}


char CSokobanView::NextNextToWorker(int d)
{
    char next2;
    switch (d)
    {
    case 0: // H: left
        next2 = Map[W_y][W_x - 2];
        break;
    case 1: // K: up
        next2 = Map[W_y - 2][W_x];
        break;
    case 2: // J: down
        next2 = Map[W_y + 2][W_x];
        break;
    case 3: // L: right
        next2 = Map[W_y][W_x + 2];
        break;
    }
    return next2;
}


void CSokobanView::moveBox(int d)
{
    switch (d)
    {
    case 0: // H: left
    {

        if (Map[W_y][W_x - 2] == 'D')
            Map[W_y][W_x - 2] = 'C';
        else
            Map[W_y][W_x - 2] = 'B';


        if (Map[W_y][W_x - 1] == 'C')
            Map[W_y][W_x - 1] = 'D';
        else
            Map[W_y][W_x - 1] = ' ';

        W_x--;
        break;
    }
    case 1: // K: up
    {
        if (Map[W_y - 2][W_x] == 'D')
            Map[W_y - 2][W_x] = 'C';
        else
            Map[W_y - 2][W_x] = 'B';

        if (Map[W_y - 1][W_x] == 'C')
            Map[W_y - 1][W_x] = 'D';
        else
            Map[W_y - 1][W_x] = ' ';

        W_y--;
        break;
    }
    case 2: // J: down
    {
        if (Map[W_y + 2][W_x] == 'D')
            Map[W_y + 2][W_x] = 'C';
        else
            Map[W_y + 2][W_x] = 'B';

        if (Map[W_y + 1][W_x] == 'C')
            Map[W_y + 1][W_x] = 'D';
        else
            Map[W_y + 1][W_x] = ' ';

        W_y++;
        break;
    }
    case 3: // L: right
    {
        if (Map[W_y][W_x + 2] == 'D')
            Map[W_y][W_x + 2] = 'C';

        else
            Map[W_y][W_x + 2] = 'B';

        if (Map[W_y][W_x + 1] == 'C')
            Map[W_y][W_x + 1] = 'D';
        else
            Map[W_y][W_x + 1] = ' ';

        W_x++;
        break;
    }
    }
}




bool CSokobanView::check_move(char next2, int d)
{
    if (next2 == ' ' || next2 == 'D')
        return true;
    return false;
}



int CSokobanView::check_box()
{
    int C_count = 0;
    for (int i = 0; i < Map.size(); i++)
    {
        for (int j = 0; j < Map[i].length(); j++)
        {
            if (Map[i][j] == 'C')
                C_count++;
        }
    }
    return C_count;
}


void CSokobanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CClientDC aDC(this); 
    CSokobanDoc* pDoc = GetDocument();
    CString Buffer;
    char next, next2;
    m_KeyStruck = nChar;


    if (m_KeyStruck != 0) 
    {
        switch (m_KeyStruck)
        {
        case 37:    // left
        {
            dir = LEFT;
            next = NextToWorker(dir);
            if (next == ' ' || next == 'D')
                moveWorker(dir);
            else if (next == 'B' || next == 'C')
            {
                next2 = NextNextToWorker(dir);
                if (check_move(next2, dir))
                    moveBox(dir);
            }
            break;
        }
        case 38:    // up
        {
            dir = UP;
            next = NextToWorker(dir);
            if (next == ' ' || next == 'D')
                moveWorker(dir);
            else if (next == 'B' || next == 'C')
            {
                next2 = NextNextToWorker(dir);
                if (check_move(next2, dir))
                    moveBox(dir);
            }
            break;
        }
        case 39:    // right
        {
            dir = RIGHT;
            next = NextToWorker(dir);
            if (next == ' ' || next == 'D')
                moveWorker(dir);
            else if (next == 'B' || next == 'C')
            {
                next2 = NextNextToWorker(dir);
                if (check_move(next2, dir))
                    moveBox(dir);
            }
            break;
        }
        case 40:    // down
        {
            dir = DOWN;
            next = NextToWorker(dir);
            if (next == ' ' || next == 'D')
                moveWorker(dir);
            else if (next == 'B' || next == 'C')
            {
                next2 = NextNextToWorker(dir);
                if (check_move(next2, dir))
                    moveBox(dir);
            }
            break;
        }
        case 32:    // space
        case 229:   
        {
            if (box == D_count)
            {
                level++;
                GameStart();
                pDoc->UpdateAllViews(NULL);
            }
            break;
        }
        case 82:    // R: restart
        {
            GameStart();
            pDoc->UpdateAllViews(NULL);
            break;
        }
        case 78:    // N: next
        {
            level++;
            GameStart();
            pDoc->UpdateAllViews(NULL);
            break;
        }
        }
        
    }

    WriteToScreen();
    
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CSokobanView::GameStart()
{
    // initialize
    Map.clear(); 
    m_KeyStruck = 0;
    box = 0;
    steps = 0;
    B_count = 0;
    D_count = 0;
    W_count = 0;
    invalid = 0;
    W_x = 0;
    W_y = 0;
    if (level < 151)
        readMap(level);
    else
        Win();
}


void CSokobanView::Win()
{
    AfxMessageBox(L"Win!!!!.", MB_OK);
}

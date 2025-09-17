
// ChildFrm.cpp: CChildFrame 類別的實作
//

#include "pch.h"
#include "framework.h"
#include "Sokoban.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// CChildFrame 建構/解構

CChildFrame::CChildFrame() noexcept
{
	// TODO: 在此加入成員初始化程式碼
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 達到修改視窗類別或樣式的目的
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 1500;	// size
	cs.cy = 1000;
	cs.x = 0;		// position
	cs.y = 0;
	return TRUE;
}

// CChildFrame 診斷

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 訊息處理常式

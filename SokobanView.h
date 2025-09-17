
// SokobanView.h: CSokobanView 類別的介面
//

#pragma once

#include <string>
#include <vector>
using std::vector;
using std::string;

class CSokobanView : public CView
{
protected: // 僅從序列化建立
	CSokobanView() noexcept;
	DECLARE_DYNCREATE(CSokobanView)

// 屬性
public:
	CSokobanDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CSokobanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	void WriteToScreen();
	int readMap(int n);
private:
	vector<string> Map;
	int B_count;
	int D_count;
	int W_count;
	int invalid;
	int W_x;
	int W_y;
	int maxblank;
	int box;
	int steps;
public:
	void showMap();
	void MaxBlank();
	char NextToWorker(int d);
	void moveWorker(int d);
	char NextNextToWorker(int d);
	void moveBox(int d);
	bool check_move(char next2, int d);
	int check_box();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	int m_KeyStruck;
	void error_message();
	void GameStart();
	void Win();
};

#ifndef _DEBUG  // 對 SokobanView.cpp 中的版本進行偵錯
inline CSokobanDoc* CSokobanView::GetDocument() const
   { return reinterpret_cast<CSokobanDoc*>(m_pDocument); }
#endif


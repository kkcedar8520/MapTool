#pragma once
// JH_ToolView.cpp: CJHToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "JH_Tool.h"
#endif

#include "JH_ToolDoc.h"
#include "JH_ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJHToolView

IMPLEMENT_DYNCREATE(CJHToolView, CView)

BEGIN_MESSAGE_MAP(CJHToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CJHToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CJHToolView 생성/소멸

CJHToolView::CJHToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CJHToolView::~CJHToolView()
{
}

BOOL CJHToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CJHToolView 그리기

void CJHToolView::OnDraw(CDC* /*pDC*/)
{
	CJHToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CJHToolView 인쇄


void CJHToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CJHToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CJHToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CJHToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CJHToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CJHToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CJHToolView 진단

#ifdef _DEBUG
void CJHToolView::AssertValid() const
{
	CView::AssertValid();
}

void CJHToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJHToolDoc* CJHToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJHToolDoc)));
	return (CJHToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CJHToolView 메시지 처리기


void CJHToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.ResizeDevice(cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
LRESULT CJHToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.WndProc(m_hWnd, message, wParam, lParam);
	return CView::WindowProc(message, wParam, lParam);
}
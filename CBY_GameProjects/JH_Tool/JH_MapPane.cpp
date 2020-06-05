#pragma once
// JH_MapPane.cpp: 구현 파일
//
#include "stdafx.h"
#include "pch.h"
#include "JH_Tool.h"
#include "JH_MapPane.h"


// JH_MapPane

IMPLEMENT_DYNAMIC(JH_MapPane, CDockablePane)

JH_MapPane::JH_MapPane()
{

}

JH_MapPane::~JH_MapPane()
{

		//if (m_WndForm)
		//{
		//	delete(JH_MapForm*)m_WndForm;
		//}

}


BEGIN_MESSAGE_MAP(JH_MapPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// JH_MapPane 메시지 처리기




void JH_MapPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);


	if (m_WndForm)
	{
		m_WndForm->SetWindowPos(NULL, 0, 0, cx, cy,
			SWP_NOZORDER);   //제트값만 빼고 넘겨준다
	}
}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.



int JH_MapPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_WndForm = JH_MapForm::CreateOne(this);
	return 0;
}


int JH_MapPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd* pParentFrame = GetParentFrame();   //부모로부터 자식 데이터를 넘겨주는데 다이어 로그가 창에 붙어다니기 때문에 마우스의 정보 오류가
													//	날수 있기때문에 MapPane을 누른것이 아니라고 알려주는 코드들이다.
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(
			pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}

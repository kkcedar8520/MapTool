// JH_ObjPane.cpp: 구현 파일
//
#pragma once

#include "stdafx.h"
#include "JH_Tool.h"
#include "JH_ObjPane.h"
#include"JH_ObjForm.cpp"

// JH_ObjPane

IMPLEMENT_DYNAMIC(JH_ObjPane, CDockablePane)

JH_ObjPane::JH_ObjPane()
{

}

JH_ObjPane::~JH_ObjPane()
{
}

int JH_ObjPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ObjForm = JH_ObjForm::CreateOne(this);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


int JH_ObjPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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




void JH_ObjPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);


	if (m_ObjForm)
	{
		m_ObjForm->SetWindowPos(NULL, 0, 0, cx, cy,
			SWP_NOZORDER);   //제트값만 빼고 넘겨준다
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다._
}

BEGIN_MESSAGE_MAP(JH_ObjPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_SIZE()
END_MESSAGE_MAP()






// JH_ObjPane 메시지 처리기
#include"pch.h"

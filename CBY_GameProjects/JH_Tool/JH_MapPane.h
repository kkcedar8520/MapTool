#pragma once


// JH_MapPane
#include"JH_MapForm.h"

class JH_MapPane : public CDockablePane
{
	DECLARE_DYNAMIC(JH_MapPane)

	JH_MapForm* m_WndForm;

public:
	JH_MapPane();
	virtual ~JH_MapPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};



#pragma once


// JH_ObjPane

#include"JH_ObjForm.h"
class JH_ObjPane : public CDockablePane
{
public:
	DECLARE_DYNAMIC(JH_ObjPane)
	JH_ObjForm* m_ObjForm;
public:
	JH_ObjPane();
	virtual ~JH_ObjPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



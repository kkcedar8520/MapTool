#pragma once


// CPropMapWnd

class CPropMapWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CPropMapWnd)

public:
	CPropMapWnd();
	virtual ~CPropMapWnd();
	CMFCPropertyGridCtrl m_wndPropList;
protected:
	DECLARE_MESSAGE_MAP()
};



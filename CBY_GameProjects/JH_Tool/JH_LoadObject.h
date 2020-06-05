#pragma once


// JH_LoadObject 대화 상자

class JH_LoadObject : public CDialogEx
{
	DECLARE_DYNAMIC(JH_LoadObject)

public:
	JH_LoadObject(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~JH_LoadObject();

public:
	std::vector<std::wstring> m_ObjList;
	void LoadResource(CString ext);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JH_LoadObject };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString m_SelectFileName;
	CListBox m_ObjListBox;
	CListBox m_ActListBox;
	
	afx_msg void OnLbnSelchangeList2();
};

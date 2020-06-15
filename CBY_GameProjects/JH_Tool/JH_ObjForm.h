#pragma once

#include<vector>
// JH_ObjForm 대화 상자

class JH_ObjForm : public CFormView
{
	DECLARE_DYNAMIC(JH_ObjForm)
public:

	static JH_ObjForm* CreateOne(CWnd* pParent);
public:
	JH_ObjForm();   // 표준 생성자입니다.
	virtual ~JH_ObjForm();
public:
	std::vector<std::wstring> m_ObjList;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JH_ObjForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton2();
	CString m_SkinName;
	CString m_BoneName;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedObjectSelect();
	bool m_bSet;

	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedRotation();
	afx_msg void OnBnClickedMove();
	afx_msg void OnBnClickedScale();
	
	afx_msg void OnBnClickedDelete();
	CListBox m_SkinListBox;
	CListBox m_BoneListBox;
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnBnClickedTransLation();
	float m_Value;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fTransX;
	float m_fTransZ;
	float m_fRotYaw;
	float m_fRotPit;
	float m_fRotRol;
	float m_fTransY;
	afx_msg void OnBnClickedBoneLoad();
	afx_msg void OnLbnObjSelChange();
	CListBox m_ObjectListBox;
	afx_msg void OnBnClickedShowinfom();
	INT m_Obj_ID;
	INT m_ObjNode;
	DWORD m_ObjFlag;
	afx_msg void OnEnChangeObjflag();
};

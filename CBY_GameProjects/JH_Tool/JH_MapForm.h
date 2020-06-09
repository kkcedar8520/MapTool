#pragma once


// JH_MapForm 대화 상자

class JH_MapForm : public CFormView
{
	DECLARE_DYNAMIC(JH_MapForm)

public:
	JH_MapForm();   // 표준 생성자입니다.
	virtual ~JH_MapForm();

	static JH_MapForm* CreateOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JH_MapForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	int m_iWidth;
	int m_iHeight;
	int m_iCellCount;
	int m_iCellSize;
	virtual void OnInitialUpdate();
	afx_msg void OnEnChangeEdit5();
	CString m_TextrueName;
	afx_msg void OnBnClickedOk2();
	CString m_NormalMapFile;
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedOk4();
	CString m_Brush;
	afx_msg void OnBnClickedOk5();
	CString m_SplattTexture1;
	CString m_SplattTexture2;
	CString m_SplattTexture3;
	CString m_SplattTexture4;
	afx_msg void OnBnClickedOk6();
	afx_msg void OnSplattTexture2BnClickedOk();
	afx_msg void OnSplattTexture3BnClickedOk();
	afx_msg void OnSplattTexture4BnClickedOk();
	float m_SplattTex1Alpha;
	float m_SplattTex2Alpha;
	float m_SplattTex3Alpha;
	float m_SplattTex4Alpha;

	int m_SplattTex1ID;
	int m_SplattTex2ID;
	int m_SplattTex3ID;
	int m_SplattTex4ID;
	//afx_msg void OnUpdateSplattTex1();
	afx_msg void OnEnChangeEdit14();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit16();
	afx_msg void OnEnChangeEdit17();
	afx_msg void OnSaveMapData();
	afx_msg void OnLoadMapData();
	CString m_SaveFileName;
	CString m_LoadFileName;
	CString m_SplattTexture;
	afx_msg void OnSplattFile20();
	afx_msg void OnMapUpDown();
	float m_HeightValue;
	afx_msg void OnMapFlatt();
	float m_fRadius;
	CString m_HeightMapFile;

};

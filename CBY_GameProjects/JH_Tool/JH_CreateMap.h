#pragma once


// JH_CreateMap 대화 상자

class JH_CreateMap : public CDialogEx
{
	DECLARE_DYNAMIC(JH_CreateMap)

public:
	JH_CreateMap(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~JH_CreateMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JH_CreateMap };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
 
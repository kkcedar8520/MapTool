// JH_ObjForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "JH_Tool.h"
#include "JH_ObjForm.h"
#include "afxdialogex.h"


// JH_ObjForm 대화 상자

IMPLEMENT_DYNAMIC(JH_ObjForm, CFormView)

JH_ObjForm::JH_ObjForm()
	: CFormView(IDD_JH_ObjForm)
	, m_SkinName(_T(""))
	, m_fSpin(0)
	, m_bSet(false)
	, m_BoneName(_T(""))
	, m_Value(0)
{

}

JH_ObjForm::~JH_ObjForm()
{
}

JH_ObjForm* JH_ObjForm::CreateOne(CWnd* pParent)//하나의 윈도우에 하나가 생성되게 만들기위한함수
{
	JH_ObjForm* pForm = new JH_ObjForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}
void JH_ObjForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_SkinName);
	DDX_Text(pDX, IDC_EDIT_SPIN, m_fSpin);
	DDX_Text(pDX, IDC_EDIT2, m_BoneName);
	DDX_Text(pDX, IDC_EDIT12, m_Value);
}
void JH_ObjForm::OnBnClickedButton2()
{
	UpdateData(FALSE);

	CString FileName;

	static int Tick = 0;

	CFileDialog dlg(FALSE, L"bmp|jpg", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"skn Files(*.skn)|*.cby|mtr Files(*.mtr)|*.cby |All Files(*.*)|*.*|", this);

	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	T_STR DirBuf;
	if (dlg.DoModal() == IDOK)
	{
		FileName = dlg.GetPathName();
		_tsplitpath(FileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		TCHAR Mdir[MAX_PATH] = L"../../data/Map/";
		TCHAR* tok = wcstok(Dir, L"\\""");
		bool b = false;
		while (tok != nullptr)
		{

			tok = wcstok(NULL, L"\\""");
			if (tok == nullptr)break;
			if (b)
			{
				DirBuf += L"/";
				DirBuf += tok;
			}
			if (StrStrW(tok, _T("data")))
			{
				b = true;
				DirBuf += L"../../";
				DirBuf += tok;
			}

		}
		DirBuf += L"/";
		_stprintf_s(szFileName, _T("%s%s%s"), DirBuf.c_str(), FName, Ext);

		if (Tick)
		{
			FileName = szFileName;

			m_BoneName = FileName;
			Tick = 0;
		}
		else
		{
			FileName = szFileName;
			Tick = 1;
			m_SkinName = FileName;
		}



		UpdateData(FALSE);
	}


	UpdateData(FALSE);
}
void JH_ObjForm::OnBnClickedButton3()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);


	pApp->m_Sample.CreateObj(
		m_SkinName,
		m_BoneName,
		mWorld);
	
	pApp->m_Sample.m_ToolState = ADDOBJECT;
}
void JH_ObjForm::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta < 0)
	{
		m_fSpin += 1.0f;
	}
	else
	{
		m_fSpin -= 1.0f;
	}

	SetDlgItemInt(IDC_EDIT_SPIN, m_fSpin);
	*pResult = 0;
}

void JH_ObjForm::OnBnClickedObjectSelect()
{
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();

	pApp->m_Sample.bSelect = true;
	pApp->m_Sample.m_ToolState = SELECT;

}
void JH_ObjForm::OnBnClickedSet()
{
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.bSet = true;


	pApp->m_Sample.m_ToolState = SETTING;


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnBnClickedRotation()
{
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	
		pApp->m_Sample.m_ToolState = ROTATION;
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnBnClickedMove()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.m_ToolState = MOVE;
	pApp->m_Sample.m_fValue = m_Value;
	if (pApp->m_Sample.m_pSelectMapObj)
	{
		pApp->m_Sample.m_pSelectMapObj->m_matWorld = pApp->m_Sample.m_ObjectList[pApp->m_Sample.m_SelectObjID]->m_matWorld;
		pApp->m_Sample.ResetSRTValue();
	}
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnBnClickedScale()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.m_ToolState = SCALE;
	pApp->m_Sample.m_fValue = m_Value;
	UpdateData(FALSE);
}
void JH_ObjForm::OnBnClickedDelete()
{

	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.ObjectDelete();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void JH_ObjForm::OnBnClickedTransLation()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	pApp->m_Sample.m_ToolState = TRANSLATION;
		pApp->m_Sample.m_fValue = m_Value;
		UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BEGIN_MESSAGE_MAP(JH_ObjForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &JH_ObjForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &JH_ObjForm::OnBnClickedButton3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &JH_ObjForm::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_SELECT, &JH_ObjForm::OnBnClickedObjectSelect)
	ON_BN_CLICKED(IDC_SET, &JH_ObjForm::OnBnClickedSet)
	ON_BN_CLICKED(IDC_ROTATION, &JH_ObjForm::OnBnClickedRotation)
	ON_BN_CLICKED(IDC_Move, &JH_ObjForm::OnBnClickedScale)
	ON_BN_CLICKED(IDC_Move2, &JH_ObjForm::OnBnClickedMove)
	ON_BN_CLICKED(IDC_Delete, &JH_ObjForm::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SELECT4, &JH_ObjForm::OnBnClickedTransLation)
END_MESSAGE_MAP()


// JH_ObjForm 메시지 처리기
#include"pch.h"







































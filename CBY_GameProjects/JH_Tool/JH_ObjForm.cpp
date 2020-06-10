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
	, m_bSet(false)
	, m_BoneName(_T(""))
	, m_Value(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
	, m_fTransX(0)
	, m_fTransZ(0)
	, m_fRotYaw(0)
	, m_fRotPit(0)
	, m_fRotRol(0)
	, m_fTransY(0)
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
	DDX_Text(pDX, IDC_EDIT2, m_BoneName);
	//DDX_Text(pDX, IDC_EDIT12, m_Value);
	DDX_Text(pDX, IDC_EDIT14, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT15, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT24, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT21, m_fTransX);
	DDX_Text(pDX, IDC_EDIT16, m_fTransZ);
	DDX_Text(pDX, IDC_EDIT22, m_fRotYaw);
	DDX_Text(pDX, IDC_EDIT23, m_fRotPit);
	DDX_Text(pDX, IDC_EDIT25, m_fRotRol);
	DDX_Text(pDX, IDC_EDIT19, m_fTransY);
	DDX_Control(pDX, IDC_LIST1, m_ObjectListBox);
}


void JH_ObjForm::OnBnClickedButton2()
{
	UpdateData(FALSE);

	CString FileName;

	static int Tick = 0;

	CFileDialog dlg(FALSE, L"bmp|jpg", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"skn Files(*.skn)|*.skn|mtr Files(*.mtr)|*.mtr|All Files(*.*)|*.*|", this);

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

	
		
			FileName = szFileName;
			Tick = 1;
			m_SkinName = FileName;
		



		UpdateData(FALSE);
	}


}

void JH_ObjForm::OnBnClickedBoneLoad()
{

	UpdateData(FALSE);

	CString FileName;

	static int Tick = 0;

	CFileDialog dlg(FALSE, L"bmp|jpg", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"mtr Files(*.mtr)|*.mtr|All Files(*.*)|*.*|", this);

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



		FileName = szFileName;
		m_BoneName = FileName;
		UpdateData(FALSE);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnBnClickedButton3()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);

	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	T_STR DirBuf;
	CString FileName;
	CString SkinName,BoneName;
	SkinName = m_SkinName;
	_tsplitpath(SkinName, Drive, Dir, FName, Ext);

	SkinName = Drive ;
	SkinName += Dir;
	SkinName += FName;
	BoneName = m_BoneName;
	_tsplitpath(BoneName, Drive, Dir, FName, Ext);
	BoneName = Drive;
	BoneName += Dir;
	BoneName += FName;

	if (BoneName == SkinName)
	{
		if (pApp->m_Sample.CreateObj(
			m_SkinName,
			m_BoneName,
			mWorld))
		{
			FileName = m_SkinName;
			_tsplitpath(FileName, Drive, Dir, FName, Ext);
		

			FileName = Drive ;
			FileName += Dir;
			FileName += FName;
			

			m_ObjectListBox.AddString(FileName);
		}
	}

	
	pApp->m_Sample.m_ToolState = ADDOBJECT;
}


void JH_ObjForm::OnBnClickedObjectSelect()
{
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();

	pApp->m_Sample.bSelect = true;
	pApp->m_Sample.m_ToolState = SELECT;
	if(pApp->m_Sample.m_pSelectMapObj)
	pApp->m_Sample.m_pSelectMapObj = nullptr;

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
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();
	

	if (!pApp->m_Sample.m_pSelectMapObj)return;
	pApp->m_Sample.m_fYaw =m_fRotYaw ;
	pApp->m_Sample.m_fPitch = m_fRotPit;
	pApp->m_Sample.m_fRoll = m_fRotRol;
	pApp->m_Sample.ObjRotation();
	pApp->m_Sample.m_QuadTree->ChangeObjectNode(pApp->m_Sample.m_pSelectMapObj);


	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnBnClickedMove()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();

	if (!pApp->m_Sample.m_pSelectMapObj)return;

	pApp->m_Sample.m_ToolState = MOVE;
	pApp->m_Sample.m_fValue = m_Value;


	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnBnClickedScale()
{
	UpdateData(TRUE);
	CJHToolApp* pApp = (CJHToolApp*)AfxGetApp();

	if (!pApp->m_Sample.m_pSelectMapObj)return;
	if (m_fScaleX != 0.0f && m_fScaleY != 0.0f && m_fScaleZ != 0.0f)
	{
		pApp->m_Sample.m_fScaleX = m_fScaleX;
		pApp->m_Sample.m_fScaleY = m_fScaleY;
		pApp->m_Sample.m_fScaleZ = m_fScaleZ;
	}
	pApp->m_Sample.ObjScale();
	pApp->m_Sample.m_QuadTree->ChangeObjectNode(pApp->m_Sample.m_pSelectMapObj);

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
	if (!pApp->m_Sample.m_pSelectMapObj)return;
	pApp->m_Sample.m_matMove._41 = m_fTransX;
	pApp->m_Sample.m_matMove._42 = m_fTransY;
	pApp->m_Sample.m_matMove._43 = m_fTransZ;
	pApp->m_Sample.ObjTranslation();
	pApp->m_Sample.m_QuadTree->ChangeObjectNode(pApp->m_Sample.m_pSelectMapObj);

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void JH_ObjForm::OnLbnObjSelChange()
{
	CString name;
	CString Ext;

	 
	int iIndex = m_ObjectListBox.GetCurSel();
	m_ObjectListBox.GetText(iIndex, name);
	Ext = L".skn";
	m_SkinName = name+Ext;
	Ext = L".mtr";
	m_BoneName = name + Ext;
	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(JH_ObjForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &JH_ObjForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &JH_ObjForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_SELECT, &JH_ObjForm::OnBnClickedObjectSelect)
	ON_BN_CLICKED(IDC_SET, &JH_ObjForm::OnBnClickedSet)
	ON_BN_CLICKED(IDC_ROTATION, &JH_ObjForm::OnBnClickedRotation)
	ON_BN_CLICKED(IDC_Move, &JH_ObjForm::OnBnClickedScale)
	ON_BN_CLICKED(IDC_Move2, &JH_ObjForm::OnBnClickedMove)
	ON_BN_CLICKED(IDC_Delete, &JH_ObjForm::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SELECT4, &JH_ObjForm::OnBnClickedTransLation)
	ON_BN_CLICKED(IDC_BUTTON4, &JH_ObjForm::OnBnClickedBoneLoad)
	ON_LBN_SELCHANGE(IDC_LIST1, &JH_ObjForm::OnLbnObjSelChange)
END_MESSAGE_MAP()


// JH_ObjForm 메시지 처리기
#include"pch.h"












































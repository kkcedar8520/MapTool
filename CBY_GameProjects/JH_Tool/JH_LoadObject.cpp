#pragma once
// JH_LoadObject.cpp: 구현 파일
//

#include "stdafx.h"
#include "JH_Tool.h"
#include "JH_LoadObject.h"
#include "afxdialogex.h"



// JH_LoadObject 대화 상자

IMPLEMENT_DYNAMIC(JH_LoadObject, CDialogEx)

JH_LoadObject::JH_LoadObject(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JH_LoadObject, pParent)
	
	, m_SelectFileName(_T(""))
{

}


JH_LoadObject::~JH_LoadObject()
{
}

void JH_LoadObject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_SelectFileName);
	DDX_Control(pDX, IDC_LIST2, m_ObjListBox);
	DDX_Control(pDX, IDC_LIST1, m_ActListBox);
}

void JH_LoadObject::LoadResource(CString ext)
{
	CFileFind cfile;

	CString strFileName;
	CString strFilePath = L"../../data/character/*."
		+ ext;

	BOOL bFind = cfile.FindFile(strFilePath);
	while (bFind)
	{
		bFind = cfile.FindNextFile();
		if (!cfile.IsDots()&&!cfile.IsDirectory())//디렉토리의 끝이아니거나 파일의 끝이아닐때
		{
			strFileName = cfile.GetFileName();
			m_ObjList.push_back(strFileName.GetBuffer());
		}

	}
}
BOOL JH_LoadObject::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadResource(L"skn");
	for (int i = 0; i < m_ObjList.size(); i++)
	{
		m_ObjListBox.AddString(m_ObjList[i].c_str());
	}
	m_SelectFileName = m_ObjList[0].c_str();
	
	m_ObjList.clear();

	LoadResource(L"act");
	
	for (int  i = 0; i < m_ObjList.size(); i++)
	{
		m_ActListBox.AddString(m_ObjList[i].c_str());
	}

	UpdateData(FALSE);
	
	m_ObjList.clear();
	
	return TRUE;
}


void JH_LoadObject::OnLbnSelchangeList2()
{

	CString name;
	int iIndex = m_ObjListBox.GetCurSel();
	m_ObjListBox.GetText(iIndex, name);
	m_SelectFileName = name;
	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(JH_LoadObject, CDialogEx)

	ON_LBN_SELCHANGE(IDC_LIST2, &JH_LoadObject::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// JH_LoadObject 메시지 처리기
#include"pch.h"




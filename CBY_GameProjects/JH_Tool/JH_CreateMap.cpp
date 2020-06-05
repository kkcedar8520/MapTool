// JH_CreateMap.cpp: 구현 파일
//
#pragma once
#include "stdafx.h"
#include "JH_Tool.h"
#include "JH_CreateMap.h"
#include "afxdialogex.h"

// JH_CreateMap 대화 상자

IMPLEMENT_DYNAMIC(JH_CreateMap, CDialogEx)

JH_CreateMap::JH_CreateMap(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JH_CreateMap, pParent)
{
}

JH_CreateMap::~JH_CreateMap()
{
}

void JH_CreateMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(JH_CreateMap, CDialogEx)
END_MESSAGE_MAP()


// JH_CreateMap 메시지 처리기
#include"pch.h"
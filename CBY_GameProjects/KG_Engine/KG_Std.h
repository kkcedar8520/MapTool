#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCKAPI_
#pragma warning(disable : 4005)

#include <crtdbg.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <time.h>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "KG_Math.h"
#include "KG_Time.h"
#include "KG_Input.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"fmod_vc.lib")
#pragma comment(lib, "winmm.lib")

#define CBY_WINDOWHEAD int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#define CBY_WINDOWRUN(Class,b,c) {Class game;  srand((unsigned)time(NULL)); if(game.InitWindow(L#b,L#c)){game.GameRun();} return 1;
#define CBY_WINDOWTAIL }
#define KGCA37_RUN(Class,b,c) CBY_WINDOWHEAD CBY_WINDOWRUN(Class,b,c) CBY_WINDOWTAIL

#define Data_CalssName(Class) Class


extern HWND g_hwnd;
extern float g_SecondTime;
extern float g_gametime;
extern int g_FPS;
extern HINSTANCE g_hInstance;
extern RECT Winrt;
extern D3DXVECTOR3 g_fPickpos;

enum KG_BOXPOSITION
{
	P_BACK = 0,
	P_FRONT,
	P_ONPLANE,
	P_SPANNING
};


struct KGCA37_Ray
{
	D3DXVECTOR3 m_Orig;
	D3DXVECTOR3 m_Dir;
	KGCA37_Ray()
	{
		m_Orig = D3DXVECTOR3(0, 0, 0);
		m_Dir = D3DXVECTOR3(0, 0, 0);

	}
};

struct KG_Box
{
	// aabb							//
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	// obb							//축에 회전된 박스
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vAxis[3];
	float fExtent[3];
	float fExtentXZ;			//y값을 고려하지 않은 상자의 대각선 길이
};

struct KG_Plane
{
	float fA, fB, fC, fD;
	inline void CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
	{
		D3DXVECTOR3 e0 = v1 - v0;
		D3DXVECTOR3 e1 = v2 - v0;
		D3DXVECTOR3 vnomal;
		D3DXVec3Cross(&vnomal, &e0, &e1);
		D3DXVec3Normalize(&vnomal, &vnomal);
		fA = vnomal.x;
		fB = vnomal.y;
		fC = vnomal.z;
		fD = -(v0.x*fA + v0.y*fB + v0.z*fC);									//평면의 방정식을 통한 ax+by+cz+d=0 공식으로 d=-(ax+by+cz)이다.
	}

	inline void CreatePlane(D3DXVECTOR3 nomal, D3DXVECTOR3 v0)
	{
		D3DXVec3Normalize(&nomal, &nomal);
		fA = nomal.x;
		fB = nomal.y;
		fC = nomal.z;
		fD = -(v0.x*fA + v0.y*fB + v0.z*fC);
	}
};

template <class C> class Singleton
{
public:
	static C& GetInstance()
	{
		static C theSingleInstance;
		return theSingleInstance;
	}
};


//////////////////////////////////////////////
//  매크로
//////////////////////////////////////////////
#define str(x) L#x
#define xstr(x) str(x)
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

static std::wstring mtw(std::string data)
{
	return  std::wstring(data.begin(), data.end());
}
static std::string wtm(std::wstring data)
{
	return std::string(data.begin(), data.end());
}
/////////////////////////////////////
typedef std::basic_string<TCHAR> T_STR;
typedef std::basic_string<wchar_t> W_STG;
typedef std::basic_string<char>  C_STR;
typedef std::vector<T_STR>		T_STR_VECTOR;
typedef std::basic_string<TCHAR>::iterator	T_ITOR;
typedef std::basic_string<wchar_t>::iterator	W_ITOR;
typedef std::basic_string<char>::iterator	C_ITOR;
typedef std::vector<T_STR>		T_ARRAY_ITOR;
typedef std::vector<DWORD>				DWORD_VECTOR;
typedef	std::vector< DWORD >::iterator	DWORD_VECTOR_ITOR;
typedef std::list<DWORD>					DWORD_LIST;
typedef std::list<DWORD>::iterator		DWORD_LIST_ITOR;
typedef std::list< HANDLE >				HANDLE_LIST;
typedef	std::list< HANDLE >::iterator	HANDLE_LIST_ITOR;

//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif




#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
#define H_RETURN(x){ if (FAILED(x)){\
			LPWSTR output;\
			WCHAR buffer[256]={0,};\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
			NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
			wsprintf(buffer,L"File=%s\nLine=%s", xstr(__FILE__),xstr(__LINE__));\
			MessageBox(NULL, buffer,output,MB_OK); return hr;}\
		}
#endif
#ifndef H
#define H(x){ if (FAILED(x)){\
			LPWSTR output;\
			WCHAR buffer[256]={0,};\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
			NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
			wsprintf(buffer,L"File=%s\nLine=%s", xstr(__FILE__),xstr(__LINE__));\
			MessageBox(NULL, buffer,output,MB_OK);}\
		}
#endif
#else
#ifndef H_RETURN
#define H_RETURN(x) (x)
#endif
#ifndef H
#define H(x) (x)
#endif
#endif
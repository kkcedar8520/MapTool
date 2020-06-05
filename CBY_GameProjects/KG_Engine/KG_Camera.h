#pragma once
#include "KG_Std.h"
#include "KG_ArcBall.h"
#include "KG_Frusutm.h"

class KG_Camera
{
public:
	KG_Frusutm m_Frustum;

public:
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_OrthoProj;
	D3DXMATRIX m_World;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_DefaultUp;
	D3DXVECTOR3 m_Look;
	D3DXVECTOR3 m_Side;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_BackPos;
	D3DXVECTOR3 m_LookDir;
	D3DXVECTOR3 m_SideDir;
	D3DXVECTOR3 m_ObjDir;

public:
	float m_fNear;
	float m_fFar;
	float m_fFovY;
	float m_fAspect;
	float m_fYaw;
	float m_fPitch;
	float m_fRoll;
	float m_fRadius;
	float m_fMinRadius;
	float m_fMaxRadius;
	float m_fSpeed;

public:
	KG_ArcBall  m_WorldArcBall;
	KG_ArcBall  m_ViewArcBall;
	int   m_nMouseWhellDelta;

public:
	bool	SetViewProj();
	void	CreateViewMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 target, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	void	CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspect);
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void	UpdateVector();
	int	CheckOBBInPlane(KG_Box& box);

public:
	KG_Camera();
	~KG_Camera();
};

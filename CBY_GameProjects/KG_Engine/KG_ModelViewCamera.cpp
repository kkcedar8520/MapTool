#include "KG_ModelViewCamera.h"

bool KG_ModelViewCamera::Frame()
{
	D3DXMATRIX mCameraRot;
	D3DXMatrixInverse(&mCameraRot, NULL, m_ViewArcBall.GetRotationMatrix());

	m_fRadius += m_nMouseWhellDelta * m_fRadius * 0.1f / 120.0f;
	m_fRadius = __min(m_fMaxRadius, m_fRadius);
	m_fRadius = __max(m_fMinRadius, m_fRadius);
	m_nMouseWhellDelta = 0;

	D3DXVECTOR3 vWorldUp, vWorldAhead;
	D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

	m_Pos = m_At - vWorldAhead * m_fRadius;

	D3DXMatrixLookAtLH(&m_View, &m_Pos, &m_At, &vWorldUp);

	D3DXMATRIX mInvView;
	D3DXMatrixInverse(&mInvView, NULL, &m_View);
	mInvView._41 = mInvView._42 = mInvView._43 = 0;

	D3DXMATRIX mModelLastRotInv;
	D3DXMatrixInverse(&mModelLastRotInv, NULL, &m_mModelLastRot);

	D3DXMATRIX mModelRot;
	mModelRot = *m_ViewArcBall.GetRotationMatrix();

	m_mModelRot = m_mModelRot * m_View * mModelLastRotInv * mModelRot * mInvView;

	m_mModelLastRot = mModelRot;

	m_mModelRot._41 = m_At.x;
	m_mModelRot._42 = m_At.y;
	m_mModelRot._43 = m_At.z;

	D3DXMATRIX mTrans;
	D3DXMatrixTranslation(&mTrans, 0, 0, 0);
	m_World = mTrans * m_mModelRot;

	UpdateVector();
	return true;
}



void KG_ModelViewCamera::Update(D3DXVECTOR4 rotvalue)
{
	D3DXQUATERNION qRot;
	D3DXMATRIX matRot;
	D3DXQuaternionRotationYawPitchRoll(&qRot, rotvalue.y, rotvalue.x, rotvalue.z);
	D3DXMatrixAffineTransformation(&matRot, 1.0f, NULL, &qRot, &m_Pos);
	D3DXMatrixInverse(&m_View, NULL, &matRot);
}

KG_ModelViewCamera::KG_ModelViewCamera()
{
	D3DXMatrixIsIdentity(&m_mModelLastRot);
}



KG_ModelViewCamera::~KG_ModelViewCamera()
{
}

#pragma once
#include "CBY_Object.h"

namespace CBY
{
	class CBY_Weapon :public CBY_Object
	{
	protected:
		int m_iCharSocket;
		int m_iObjSocket;
		int m_iFireSocket;
		D3DXVECTOR3 m_vFirePos;
		D3DXVECTOR3 m_vMovePos;

	protected:
		std::vector<int> m_SocketList;
		int m_ObjIndex;
		D3DXMATRIX m_matCalculation;
		CBY_MeshSkin* m_RootMesh;

	public:
		virtual bool Update(D3DXMATRIX* parmat);
		virtual void SetMatrix(D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)override;

	public:
		void SetSocket(int i);		//ĳ���Ϳ� ����� ĳ������ ����
		void SetObjSocke(int i);	//������Ʈ�� ����
		void SetFireSocket(int i);
		inline void SetMovePos(D3DXVECTOR3 pos) { m_vMovePos = pos; }

	public:
		int GetSocket();
		int GetObjSocke();	//������Ʈ�� ����
		int GetFireSocket();
		D3DXVECTOR3 GetFirePos();

	public:
		CBY_Weapon();
		virtual ~CBY_Weapon();
	};
}



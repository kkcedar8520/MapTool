#pragma once
#include "KG_DxHeper.h"

enum ConstantBufferType
{
	CB_DYNAMIC = 0,
	CB_DEFAULT
};

class KG_DxHelperEx
{
public:
	ID3D11Device*			m_pd3dDevice;		//����̽�
	ID3D11DeviceContext*	m_pContext;			//Context �������� �ʿ�

public:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;				//���ؽ� ����
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;				//�ε��� ����
	//ID3D11Buffer* m_pConstantBuffer;			
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;	//�������

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pVertexLayout;			//���ؽ� ���۹� ���̾ƿ�
	Microsoft::WRL::ComPtr<ID3D11SamplerState>  m_pSamplerState;


	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;					//���� ���̴�
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVSmat;					//���� ���̴�
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;					//�ȼ� ���̴�
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPSBlend;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_pSRV;


	Microsoft::WRL::ComPtr<ID3DBlob> m_pVertexCode;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pPixelCode;

public:
	UINT m_VertexSize;				//���� ũ��
	UINT m_Vertexnum;				//���� ����
	UINT m_Indexnum;				//�ε��� ����
	int m_StartIndex;				//��ο� �ε����� �ε��� ���� ��ȣ
	INT iCBBufferType;

public:
	void PrePender();
	void Pender();
	void PostPender();

public:
	KG_DxHelperEx();
	~KG_DxHelperEx();
};



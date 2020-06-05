#include "KG_DxHelperEx.h"

void KG_DxHelperEx::PrePender()
{
	UINT offset = 0;
	UINT stride = m_VertexSize;
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);			//vertexbuffer�� ����, �������� ���۽���, ����, �ּ�, �迭�� �ּ�, ������ �迭�Ÿ�
	m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			//�ε��� ���� ���� �ε��� ���۴� ���� ���۸� �ѱ�°� �ƴ϶� ����s�� �ƴϰ� �ּҸ� ���� �ʿ䵵 ����


	m_pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	m_pContext->PSSetShader(m_pPS.Get(), NULL, 0);

	m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	m_pContext->IASetInputLayout(m_pVertexLayout.Get());

	//m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);			//������� �׸��� �Է�

	m_pContext->PSSetShaderResources(0, 1,m_pSRV.GetAddressOf());

}

void KG_DxHelperEx::Pender()
{
	PrePender();
	PostPender();
}

void KG_DxHelperEx::PostPender()
{
	if (m_Indexnum > 0)
	{
		m_pContext->DrawIndexed(m_Indexnum, m_StartIndex, 0);									//�ε��� ���۷� �׸����� �ε��� ������ �ε��� ������ �����ּ�, ���ؽ� ���� ���� �ָ� �ȴ�.
	}
	else
	{
		m_pContext->Draw(m_Vertexnum, 0);
	}
}


KG_DxHelperEx::KG_DxHelperEx()
{
	m_StartIndex = 0;
	m_VertexSize = 0;
	m_Vertexnum = 0;
	m_Indexnum = 0;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	m_pConstantBuffer = nullptr;
	m_pVS = nullptr;
	m_pPS = nullptr;
	m_pSRV = nullptr;
	m_pVertexCode = nullptr;
	m_pPixelCode = nullptr;

}


KG_DxHelperEx::~KG_DxHelperEx()
{
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pIndexBuffer)m_pIndexBuffer->Release();
	if (m_pVertexLayout)m_pVertexLayout->Release();
	//if (m_pConstantBuffer)m_pConstantBuffer->Release();
	if (m_pVS)m_pVS->Release();
	if (m_pPS)m_pPS->Release();
	if (m_pSRV)m_pSRV->Release();
	//if (m_pStructSRV)m_pStructSRV->Release();
	if (m_pVertexCode)m_pVertexCode->Release();
	if (m_pPixelCode)m_pPixelCode->Release();

	//if (m_pBufResult)m_pBufResult->Release();
	//if (m_pBufResultUAV)m_pBufResultUAV->Release();
	//if (m_pCS)m_pCS->Release();
}

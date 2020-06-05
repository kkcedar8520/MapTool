#include "KG_DxHelperEx.h"

void KG_DxHelperEx::PrePender()
{
	UINT offset = 0;
	UINT stride = m_VertexSize;
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);			//vertexbuffer를 셋팅, 정점버퍼 시작슬롯, 갯수, 주소, 배열의 주소, 오프셋 배열거리
	m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			//인덱스 버퍼 셋팅 인덱스 버퍼는 여러 버퍼를 넘기는게 아니라 버퍼s가 아니고 주소를 보낼 필요도 없다


	m_pContext->VSSetShader(m_pVS.Get(), NULL, 0);
	m_pContext->PSSetShader(m_pPS.Get(), NULL, 0);

	m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	m_pContext->IASetInputLayout(m_pVertexLayout.Get());

	//m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);			//어떤식으로 그릴지 입력

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
		m_pContext->DrawIndexed(m_Indexnum, m_StartIndex, 0);									//인덱스 버퍼로 그릴때는 인덱스 정보와 인덱스 데이터 시작주소, 버텍스 시작 같이 주면 된다.
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

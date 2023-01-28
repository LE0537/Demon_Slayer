#include "..\Public\Cell.h"
#include "VIBuffer_Navigation.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{	
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);	
}

HRESULT CCell::Initialize(const _float3 * pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	_float3		vLines[LINE_END];

	XMStoreFloat3(&vLines[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	m_vNormals[LINE_AB] = _float3(vLines[LINE_AB].z * -1.f, 0.f, vLines[LINE_AB].x);

	XMStoreFloat3(&vLines[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	m_vNormals[LINE_BC] = _float3(vLines[LINE_BC].z * -1.f, 0.f, vLines[LINE_BC].x);

	XMStoreFloat3(&vLines[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
	m_vNormals[LINE_CA] = _float3(vLines[LINE_CA].z * -1.f, 0.f, vLines[LINE_CA].x);

	m_pVIBuffer = CVIBuffer_Navigation::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	
	return S_OK;
}

_bool CCell::Compare_Points(const _float3 * pSourPoint, const _float3 * pDestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pSourPoint)))	
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
			return true;
	
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
			return true;		
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
			return true;

		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		if (0 < XMVectorGetX(XMVector3Dot(vDir, XMVector3Normalize(XMLoadFloat3(&m_vNormals[i])))))
		{
			*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}
	return true;
}

_fvector CCell::Sliding_Wall(_fvector vPosition, _fvector vLook)
{
	_vector vSliding = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		if (0.f < XMVectorGetX(XMVector3Dot(vDir, XMLoadFloat3(&m_vNormals[i]))))
		{
			//	90 > angle -> �ݴ�
			_vector vLine = XMVectorSet(m_vNormals[i].z, 0.f, m_vNormals[i].x * -1.f, 1.f);
			_vector vTemp = XMLoadFloat3(&m_vPoints[i == 2 ? 0 : i + 1]) - XMLoadFloat3(&m_vPoints[i]);

			_float fSlidingPower = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook), XMVector3Normalize(vLine)));

			vSliding = XMVector3Normalize(vLine) * fSlidingPower;
			break;
		}
	}

	return vSliding;
}



HRESULT CCell::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;	

	m_pVIBuffer->Render();

	return S_OK;
}


CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3 * pPoints, _int  iIndex)
{
	CCell*	pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		ERR_MSG(TEXT("Failed to Created : CCell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{

	Safe_Release(m_pVIBuffer);	


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

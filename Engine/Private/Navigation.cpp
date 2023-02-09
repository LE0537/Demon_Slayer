#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
	, m_pShader(rhs.m_pShader)
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

	Safe_AddRef(m_pShader);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar * pNavigationData)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pNavigationData, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;	

	_float3		vPoints[3];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*			pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);		
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;


	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;


	return S_OK;
}

HRESULT CNavigation::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_NaviDesc, pArg, sizeof m_NaviDesc);



	return S_OK;
}

_bool CNavigation::isMove(_fvector vPosition, _fvector vMoveDir, _Out_ _float3 * pOut)
{
	_int		iNeighborIndex = -1;

	/* 현재 존재하는 쎌안에서 움직였다. */
	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(vPosition, &iNeighborIndex))
	{
		/*	if (m_bHeight == true)
			{
				Navigation_Height(vPosition);
				m_bHeight = false;
			}*/

		Navigation_Height(vPosition);

		return true;
	}

	/* 현재 존재하는 쎌을 벗어난다.  */
	else
	{
		/* 나간 방향에 이웃셀이 존재한다. */
		int		iPreIndex = m_NaviDesc.iCurrentCellIndex;
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
				{
					if (-1 != iPreIndex)
					{
						if (0.f == XMVectorGetX(XMVector3Length(XMLoadFloat3(pOut))))
							XMStoreFloat3(pOut, m_Cells[iPreIndex]->Sliding_Wall(vPosition, vMoveDir));
						else
						{
							m_NaviDesc.iCurrentCellIndex = iPreIndex;
							return true;
						}
					}

					return false;
				}

				iPreIndex = iNeighborIndex;
				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
					break;
			}
			
			m_NaviDesc.iCurrentCellIndex = iNeighborIndex;

			return true;
		}

		/* 나간 방향에 이웃셀이 존재하지않는다. . */
		else
		{
			/*슬라이딩을 위한 리턴을 정의해도 된다. */		
			XMStoreFloat3(pOut, m_Cells[m_NaviDesc.iCurrentCellIndex]->Sliding_Wall(vPosition, vMoveDir));

			return false;
		}
		
		
	}
	

	return false;
}

_float3 * CNavigation::Get_CellPoints()
{
	return m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Points();
}


void CNavigation::Navigation_Height(_fvector vecPosition)
{
	_float fDistance = 0.f;

	_vector vRayPos = vecPosition;

	vRayPos += XMVectorSet(0.f, 10.f, 0.f, 1.f);
	vRayPos = XMVectorSetW(vRayPos, 1.f);

	_vector vRayDir = XMVectorSet(0.f, -1.f, 0.f, 0.f);

	for (_uint i = 0; i < m_Cells.size(); i++)
	{
		_vector vPointA = XMLoadFloat3(m_Cells[i]->Get_Point(CCell::POINT::POINT_A));
		vPointA = XMVectorSetW(vPointA, 1.f);
		_vector vPointB = XMLoadFloat3(m_Cells[i]->Get_Point(CCell::POINT::POINT_B));
		vPointB = XMVectorSetW(vPointB, 1.f);
		_vector vPointC = XMLoadFloat3(m_Cells[i]->Get_Point(CCell::POINT::POINT_C));
		vPointC = XMVectorSetW(vPointC, 1.f);

		if (TriangleTests::Intersects(vRayPos, vRayDir, vPointA, vPointB, vPointC, fDistance))
		{
			_vector vHeight = vRayPos + vRayDir * fDistance;
			XMStoreFloat4(&m_vPlayerHeight, vHeight);
		}
	}



}

HRESULT CNavigation::Render()
{
	_float4x4			WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());


	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	m_pShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));	

	RELEASE_INSTANCE(CPipeLine);

	if (-1 == m_NaviDesc.iCurrentCellIndex)
	{
		m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
		m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));
		m_pShader->Begin(0);

		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
			{
				pCell->Render();
			}
		}
	}

	else
	{
		WorldMatrix._24 = 0.1f;
		m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
		m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));

		m_pShader->Begin(0);
		m_Cells[m_NaviDesc.iCurrentCellIndex]->Render();
	}

	return S_OK;
}

void CNavigation::Find_CurrentCellIndex(_vector vPos)
{
	_int		iNeighborIndex = -1;

	for (_int i = 0; i < m_Cells.size(); ++i)
	{
		if (m_Cells[i]->isIn(vPos, &iNeighborIndex))
		{
			m_NaviDesc.iCurrentCellIndex = i;
			return;
		}
	}
}

_bool CNavigation::Cheak_Cell(_vector vPos)
{
	_int		iNeighborIndex = -1;

	for (_int i = 0; i < m_Cells.size(); ++i)
	{
		if (m_Cells[i]->isIn(vPos, &iNeighborIndex))
		{
			m_NaviDesc.iCurrentCellIndex = i;
			return true;
		}
	}
	return false;
}

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pNavigationData)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationData)))
	{
		ERR_MSG(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

	Safe_Release(m_pShader);

}

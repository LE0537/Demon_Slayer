#include "..\Public\VIBuffer_Line.h"

CVIBuffer_Line::CVIBuffer_Line(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Line::CVIBuffer_Line(const CVIBuffer_Line & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Line::Initialize_Prototype()
{
	m_vPos1 = _float3(0.f, 0.f, 0.f);
	m_vPos2 = _float3(0.f, 0.f, 1.f);
#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXPOS);
	m_iNumVertices = 2;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOS*		pVertices = new VTXPOS[m_iNumVertices];

	pVertices[0].vPosition = m_vPos1;
	pVertices[1].vPosition = m_vPos2;
	
	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(LINEINDICES16);
	m_iNumPrimitive = 1;
	m_iNumIndicesPerPrimitive = 2;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	LINEINDICES16*			pIndices = new LINEINDICES16[m_iNumPrimitive];

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Line::Initialize(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Line::Update(_float3 vPos1, _float3 vPos2)
{
	m_vPos1 = vPos1;
	m_vPos2 = vPos2;

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);
	((VTXPOS*)MappedSubResource.pData)[0].vPosition = m_vPos1;
	((VTXPOS*)MappedSubResource.pData)[1].vPosition = m_vPos2;
	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_Line * CVIBuffer_Line::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Line*	pInstance = new CVIBuffer_Line(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Line::Clone(void * pArg)
{
	CVIBuffer_Line*	pInstance = new CVIBuffer_Line(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Line::Free()
{
	__super::Free();
}

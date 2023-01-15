#include "..\Public\VIBuffer_NewTerrain.h"

#include "Picking.h"

CVIBuffer_NewTerrain::CVIBuffer_NewTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_NewTerrain::CVIBuffer_NewTerrain(const CVIBuffer_NewTerrain & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_NewTerrain::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_NewTerrain::Initialize(void * pArg)
{
#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	TERRAINDESC		tTerrainDesc;
	ZeroMemory(&tTerrainDesc, sizeof(TERRAINDESC));
	memcpy(&tTerrainDesc, pArg, sizeof(TERRAINDESC));

	if (0 != strlen(tTerrainDesc.strFileName))
	{
		if (FAILED(Load_Heightmap(&tTerrainDesc)))
			return E_FAIL;
		else
		{
			memcpy(pArg, &tTerrainDesc, sizeof(TERRAINDESC));
			return S_OK;
		}
	}

	m_iNumVerticesX = tTerrainDesc.iSizeX;
	m_iNumVerticesZ = tTerrainDesc.iSizeZ;

	if (0 == m_iNumVerticesX * m_iNumVerticesZ)
		return E_FAIL;


	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	m_pVertices = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			m_pVertices[iIndex].vPosition = _float3(_float(j), 0.f, _float(i));
			m_pVertices[iIndex].vNormal = _float3(0.f, 1.f, 0.f);
			m_pVertices[iIndex].vTexture = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}
#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;



	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			m_pIndices[iNumFaces]._0 = iIndices[0];
			m_pIndices[iNumFaces]._1 = iIndices[1];
			m_pIndices[iNumFaces]._2 = iIndices[2];

			_vector		vSourDir, vDestDir, vNormal;

			vSourDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;

			m_pIndices[iNumFaces]._0 = iIndices[0];
			m_pIndices[iNumFaces]._1 = iIndices[2];
			m_pIndices[iNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&m_pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[i].vNormal)));




	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_NewTerrain::Load_Heightmap(TERRAINDESC* tTerrainDesc)
{
	_tchar pHeightMapFilePath[MAX_PATH] = L"";
	char strFilePath[MAX_PATH] = "../Bin/Resources/Map/HeightMap/";
	strcat_s(strFilePath, tTerrainDesc->strFileName);
	strcat_s(strFilePath, ".bmp");

	MultiByteToWideChar(CP_ACP, 0, strFilePath, strlen(strFilePath), pHeightMapFilePath, MAX_PATH);

	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;
	_ulong*					pPixel = nullptr;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	m_iNumVerticesX = (*tTerrainDesc).iSizeX = ih.biWidth;
	m_iNumVerticesZ = (*tTerrainDesc).iSizeZ = ih.biHeight;
	

	pPixel = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);

#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	m_pVertices = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			m_pVertices[iIndex].vPosition = _float3(_float(j), (pPixel[iIndex] & 0x000000ff) / 2.f, _float(i));
			m_pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			m_pVertices[iIndex].vTexture = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}



		


#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;



	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			m_pIndices[iNumFaces]._0 = iIndices[0];
			m_pIndices[iNumFaces]._1 = iIndices[1];
			m_pIndices[iNumFaces]._2 = iIndices[2];

			_vector		vSourDir, vDestDir, vNormal;

			vSourDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;

			m_pIndices[iNumFaces]._0 = iIndices[0];
			m_pIndices[iNumFaces]._1 = iIndices[2];
			m_pIndices[iNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&m_pVertices[m_pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&m_pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[i].vNormal)));




	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

#pragma endregion

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CVIBuffer_NewTerrain::Check_Sphere_Pos(_float3 vDescPos, _float3 vSpherePos, _float fRadius, _float* fOut)
{
	_float3	vTemp;
	XMStoreFloat3(&vTemp, XMLoadFloat3(&vSpherePos) - XMLoadFloat3(&vDescPos));
	vTemp.y = vDescPos.y;
	_vector vZeroYPicking = XMLoadFloat3(&vTemp);

	_float fDist = XMVectorGetX(XMVector3Length(vZeroYPicking));
	if (fDist > fRadius)
		return E_FAIL;

	*fOut = fDist;
	return S_OK;
}

CVIBuffer_NewTerrain * CVIBuffer_NewTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_NewTerrain*	pInstance = new CVIBuffer_NewTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_NewTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_NewTerrain::Clone(void * pArg)
{
	CVIBuffer_NewTerrain*	pInstance = new CVIBuffer_NewTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_NewTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_NewTerrain::Free()
{
	if (true == m_isCloned)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}

	__super::Free();
}

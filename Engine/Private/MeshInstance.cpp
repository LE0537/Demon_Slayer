#include "..\Public\MeshInstance.h"
#include "HierarchyNode.h"
#include "Frustum.h"

CMeshInstance::CMeshInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{

}

CMeshInstance::CMeshInstance(const CMeshInstance & rhs)
	: CVIBuffer_Instance(rhs)
	, m_pAIMesh(rhs.m_pAIMesh)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_pBinAIMesh(rhs.m_pBinAIMesh)
	, m_bBinMesh(rhs.m_bBinMesh)
{
	strcpy_s(m_szName, rhs.m_szName);
}

void CMeshInstance::Get_BoneMatrices(_float4x4 * pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_iNumBones)
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	_uint		iNumBones = 0;

	for (auto& pBoneNode : m_Bones)
	{
		/* Offset * Combined * Pivot */
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], (pBoneNode->Get_OffsetMatrix() * pBoneNode->Get_CombinedTransformationMatrix() * PivotMatrix));
	}
}

HRESULT CMeshInstance::Initialize_Prototype(CModel::TYPE eModelType, const aiMesh * pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_pAIMesh = pAIMesh;

#pragma region VERTICES

	if (CModel::TYPE_NONANIM_INSTANCING != eModelType)
		return E_FAIL;


	HRESULT	hr = Create_VertexBuffer_NonAnimModel(pAIMesh, PivotMatrix);

	if (FAILED(hr))
		return E_FAIL;


#pragma endregion


	return S_OK;
}

HRESULT CMeshInstance::Bin_Initialize_Prototype(CModel::TYPE eModelType, DATA_BINMESH * pAIMesh, CModel * pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->cName);

	m_iMaterialIndex = pAIMesh->iMaterialIndex;
	m_pBinAIMesh = pAIMesh;

	m_bBinMesh = true;

#pragma region VERTICES

	if (CModel::TYPE_NONANIM_INSTANCING != eModelType)
		return E_FAIL;


	HRESULT	hr = Bin_Create_VertexBuffer_NonAnimModel(pAIMesh, PivotMatrix);

	if (FAILED(hr))
		return E_FAIL;


#pragma endregion

	return S_OK;
}

HRESULT CMeshInstance::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return S_OK;

	CModel::MESHINSTANCINGDESC tMeshInstancingDesc;
	memcpy(&tMeshInstancingDesc, pArg, sizeof tMeshInstancingDesc);
	m_iNumInstance = tMeshInstancingDesc.iNumMeshInstancing;

#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	_int			iNumFaces = 0;
	if (false == m_bBinMesh)
		iNumFaces = m_pAIMesh->mNumFaces;
	else
		iNumFaces = m_pBinAIMesh->iNumPrimitives;		//	Binary화 된 파일의 iNumPrimitive는 iNumFaces 이다.

	m_iNumPrimitive = iNumFaces * m_iNumInstance;

	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint j = 0; j < m_iNumInstance; ++j)
	{
		for (_uint i = 0; i < iNumFaces; ++i)
		{
			if (false == m_bBinMesh)
			{
				aiFace		AIFace = m_pAIMesh->mFaces[i];

				_int		iIndex = i + (j * iNumFaces);
				pIndices[iIndex]._0 = AIFace.mIndices[0];
				pIndices[iIndex]._1 = AIFace.mIndices[1];
				pIndices[iIndex]._2 = AIFace.mIndices[2];
			}
			else
			{
				_int		iIndex = i + (j * iNumFaces);
				pIndices[iIndex]._0 = m_pBinAIMesh->pIndices[i]._0;
				pIndices[iIndex]._1 = m_pBinAIMesh->pIndices[i]._1;
				pIndices[iIndex]._2 = m_pBinAIMesh->pIndices[i]._2;
			}
		}
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iInstanceStride = sizeof(VTXMATRIX);
	m_iNumVertices = m_iNumInstance;
	m_iNumIndicesPerInstance = m_iNumIndicesPerPrimitive * m_pBinAIMesh->iNumPrimitives;

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXMATRIX*			pInstanceVtx = new VTXMATRIX[m_iNumVertices];
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVtx[i].vRight = _float4(0.5f, 0.f, 0.f, 0.f);
		pInstanceVtx[i].vUp = _float4(0.f, 0.5f, 0.f, 0.f);
		pInstanceVtx[i].vLook = _float4(0.f, 0.f, 0.5f, 0.f);
		pInstanceVtx[i].vPosition = _float4(_float(i) * 0.5, 0.0f, _float(i) * 0.5, 1.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstanceVtx;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVtx);
#pragma endregion


	return S_OK;
}

HRESULT CMeshInstance::Render()
{
	ID3D11Buffer*		pBuffer[] = {
		m_pVB,
		m_pInstanceBuffer
	};

	_uint				iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffer, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	m_pContext->DrawIndexedInstanced(m_iNumIndicesPerInstance, m_iNumInstance, 0, 0, 0);


	return S_OK;
}

void CMeshInstance::Update(vector<VTXMATRIX> vecMatrix, _float fRadiusRatio, _float fTimeDelta)
{
	vector<VTXMATRIX>	vecRenderMatrix;

	CFrustum*	pFrustum= GET_INSTANCE(CFrustum);
	for (auto & iter : vecMatrix)
	{
		_float	fLength = fRadiusRatio * max(max(XMVectorGetX(XMVector3Length(XMLoadFloat4(&iter.vRight))), XMVectorGetX(XMVector3Length(XMLoadFloat4(&iter.vUp)))), XMVectorGetX(XMVector3Length(XMLoadFloat4(&iter.vLook))));

		if (true == pFrustum->IsinFrustum(XMLoadFloat4(&iter.vPosition), fLength))
			vecRenderMatrix.push_back(iter);
	}
	
	m_iNumRendering = vecRenderMatrix.size();

	_int			iNumFaces = 0;
	if (false == m_bBinMesh)
		iNumFaces = m_pAIMesh->mNumFaces;

	if (false == m_bBinMesh)
		m_iNumPrimitive = iNumFaces * m_iNumRendering;
	else
		m_iNumPrimitive = (m_pBinAIMesh->iNumPrimitives) * m_iNumRendering;

	m_iNumIndicesPerInstance = m_iNumIndicesPerPrimitive * m_pBinAIMesh->iNumPrimitives;

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumRendering; ++i)
	{
		//	하고싶은것
		((VTXMATRIX*)MappedSubResource.pData)[i].vRight = vecRenderMatrix[i].vRight;
		((VTXMATRIX*)MappedSubResource.pData)[i].vUp = vecRenderMatrix[i].vUp;
		((VTXMATRIX*)MappedSubResource.pData)[i].vLook = vecRenderMatrix[i].vLook;
		((VTXMATRIX*)MappedSubResource.pData)[i].vPosition = vecRenderMatrix[i].vPosition;
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	RELEASE_INSTANCE(CFrustum);
}

HRESULT CMeshInstance::SetUp_Bones(CModel * pModel)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*		pAIBone = m_pAIMesh->mBones[i];

		CHierarchyNode*	pBoneNode = pModel->Get_BonePtr(pAIBone->mName.data);
		if (nullptr == pBoneNode)
			return E_FAIL;

		m_Bones.push_back(pBoneNode);

		Safe_AddRef(pBoneNode);

		_float4x4		OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBoneNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
	}


	if (0 == m_iNumBones)
	{
		CHierarchyNode*		pNode = pModel->Get_BonePtr(m_szName);
		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

HRESULT CMeshInstance::Create_VertexBuffer_NonAnimModel(const aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 2;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexUV, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshInstance::Create_VertexBuffer_AnimModel(const aiMesh* pAIMesh, CModel* pModel)
{
/*	초기화 필요
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));


	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMODEL*			pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));
	}

	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			_uint		iVertexIndex = pAIBone->mWeights[j].mVertexId;

			if (0 == pVertices[iVertexIndex].vBlendWeight.x)
			{
				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.x = i;
			}

			else if (0 == pVertices[iVertexIndex].vBlendWeight.y)
			{
				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.y = i;
			}

			else if (0 == pVertices[iVertexIndex].vBlendWeight.z)
			{
				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.z = i;
			}

			else if (0 == pVertices[iVertexIndex].vBlendWeight.w)
			{
				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.w = i;
			}
		}
	}


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	*/

	return S_OK;

}

HRESULT CMeshInstance::Bin_Create_VertexBuffer_NonAnimModel(DATA_BINMESH * pAIMesh, _fmatrix PivotMatrix)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->NumVertices;
	m_iNumVertexBuffers = 2;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i] = pAIMesh->pNonAnimVertices[i];
		memcpy(&pVertices[i].vPosition, &pAIMesh->pNonAnimVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->pNonAnimVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));
		/*
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));
		*/
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMeshInstance * CMeshInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, const aiMesh * pAIMesh, CModel * pModel, _fmatrix PivotMatrix)
{
	CMeshInstance*	pInstance = new CMeshInstance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshInstance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMeshInstance * CMeshInstance::Bin_Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, DATA_BINMESH * pAIMesh, CModel * pModel, _fmatrix PivotMatrix)
{
	CMeshInstance*			pInstance = new CMeshInstance(pDevice, pContext);

	if (FAILED(pInstance->Bin_Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed To Created : CMeshInstance_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshInstance::Clone(void * pArg)
{
	CMeshInstance*	pInstance = new CMeshInstance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshInstance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshInstance::Free()
{
	__super::Free();

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);

	m_Bones.clear();

}

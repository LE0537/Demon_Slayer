#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::Initialize(const aiNode * pNode, CHierarchyNode* pParent)
{

	strcpy_s(m_szName, pNode->mName.data);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	/* 언제든 바뀔 수 있다. 애니메이션이 재생되면. */
	memcpy(&m_TransformationMatrix, &pNode->mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	m_OriTransformationMatrix = m_TransformationMatrix; // 추가

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());


	m_pParent = pParent;

	Safe_AddRef(m_pParent);

	return S_OK;
}

void CHierarchyNode::Invalidate_CombinedTransformationmatrix(_bool bRemoveTranslation)
{
	//if (bRemoveTranslation == true)
	//{
	//	if (!strcmp(m_szName, "Root"))
	//	{
	//		_float4 OriginMatrixTranslation; XMStoreFloat4(&OriginMatrixTranslation, XMLoadFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0]));
	//		XMStoreFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0], XMVectorSet(OriginMatrixTranslation.x, OriginMatrixTranslation.y, OriginMatrixTranslation.z, 1.f));
	//		m_RootNodeMatrix = m_CombinedTransformationMatrix;
	//	}
	//}

	//if (nullptr != m_pParent)
	//{
	//	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	//}
	//else
	//	m_CombinedTransformationMatrix = m_TransformationMatrix;


	//if (!strcmp(m_szName, "Root"))
	//{
	//	if (bRemoveTranslation == true)
	//	{
	//		_float4 OriginMatrixTranslation; XMStoreFloat4(&OriginMatrixTranslation, XMLoadFloat4(&*(_float4*)&m_RootNodeMatrix.m[3][0]));
	//		XMStoreFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0], XMVectorSet(OriginMatrixTranslation.x, OriginMatrixTranslation.y, OriginMatrixTranslation.z, 1.f));
	//	}
	//	else
	//	{
	//		_float4 OriginMatrixTranslation; XMStoreFloat4(&OriginMatrixTranslation, XMLoadFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0]));
	//		XMStoreFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0], XMVectorSet(0.f, OriginMatrixTranslation.y, 0.f, 1.f));
	//	}
	//		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));

	//}
	if (!strcmp(m_szName, "Root"))
	{
		//m_MoveTransformationMatrix = m_TransformationMatrix;

		_matrix OriginMatrixTranslation = XMLoadFloat4x4(&m_TransformationMatrix);

		OriginMatrixTranslation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		XMStoreFloat4x4(&m_TransformationMatrix, OriginMatrixTranslation);
	}

	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	else
		m_CombinedTransformationMatrix = m_TransformationMatrix;
}

void CHierarchyNode::RuiDad_Invalidate_CombinedTransformationmatrix(_bool bRemoveTranslation)
{
	//if (!strcmp(m_szName, "C_Hips_1"))
	//{
	//	//m_MoveTransformationMatrix = m_TransformationMatrix;

	//	_matrix OriginMatrixTranslation = XMLoadFloat4x4(&m_TransformationMatrix);

	//	OriginMatrixTranslation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//	XMStoreFloat4x4(&m_TransformationMatrix, OriginMatrixTranslation);
	//}

	if (!strcmp(m_szName, "Root"))
	{
		//m_MoveTransformationMatrix = m_TransformationMatrix;

		_matrix OriginMatrixTranslation = XMLoadFloat4x4(&m_TransformationMatrix);

		OriginMatrixTranslation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		XMStoreFloat4x4(&m_TransformationMatrix, OriginMatrixTranslation);
	}

	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	else
		m_CombinedTransformationMatrix = m_TransformationMatrix;




}

void CHierarchyNode::Invalidate_CombinedTransformationmatrix(_bool bRemoveTranslation, _fvector vPosition)
{
	if (bRemoveTranslation == true)
	{
		if (!strcmp(m_szName, "Root"))
		{
			_float4 OriginMatrixTranslation; XMStoreFloat4(&OriginMatrixTranslation, (vPosition));
			XMStoreFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0], XMVectorSet(OriginMatrixTranslation.x, OriginMatrixTranslation.y, OriginMatrixTranslation.z, 1.f));
			m_RootNodeMatrix = m_CombinedTransformationMatrix;
		}
	}

	if (nullptr != m_pParent)
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	}
	else
		m_CombinedTransformationMatrix = m_TransformationMatrix;


	if (!strcmp(m_szName, "Root"))
	{
		if (bRemoveTranslation == true)
		{
			_float4 OriginMatrixTranslation; XMStoreFloat4(&OriginMatrixTranslation, XMLoadFloat4(&*(_float4*)&m_RootNodeMatrix.m[3][0]));
			XMStoreFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0], XMVectorSet(OriginMatrixTranslation.x, OriginMatrixTranslation.y, OriginMatrixTranslation.z, 1.f));
		}
		else
		{
			_float4 OriginMatrixTranslation; XMStoreFloat4(&OriginMatrixTranslation, XMLoadFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0]));
			XMStoreFloat4(&*(_float4*)&m_CombinedTransformationMatrix.m[3][0], XMVectorSet(0.f, OriginMatrixTranslation.y, 0.f, 1.f));
		}
		//XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));

	}
	//if (!strcmp(m_szName, "C_Hips_1"))
	//{
	//	//m_MoveTransformationMatrix = m_TransformationMatrix;

	//	_matrix matBoneTransformation = XMLoadFloat4x4(&m_TransformationMatrix);

	//	matBoneTransformation.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//	XMStoreFloat4x4(&m_TransformationMatrix, matBoneTransformation);
	//}

	//if (nullptr != m_pParent)
	//	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	//else
	//	m_CombinedTransformationMatrix = m_TransformationMatrix;
}

CHierarchyNode * CHierarchyNode::Create(const aiNode* pNode, CHierarchyNode* pParent)
{
	CHierarchyNode*		pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pNode, pParent)))
	{
		ERR_MSG(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}
	return pInstance;
}
HRESULT CHierarchyNode::Bin_Initialize(DATA_BINNODE * pNode)
{
	strcpy_s(m_szName, pNode->cName);
	strcpy_s(m_szParentName, pNode->cParent);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	memcpy(&m_TransformationMatrix, &pNode->mTransform, sizeof(_float4x4));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}
void CHierarchyNode::Set_FindParent(CHierarchyNode * pNode)
{
	const char* pName = pNode->Get_Name();
	if (!strcmp(m_szParentName, pName))
	{
		m_pParent = pNode;
		Safe_AddRef(m_pParent);
		return;
	}
	m_pParent = nullptr;
}
CHierarchyNode * CHierarchyNode::Bin_Create(DATA_BINNODE * pNode)
{
	CHierarchyNode*			pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Bin_Initialize(pNode)))
	{
		ERR_MSG(TEXT("Failed To Created : CHierarchyNode_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}
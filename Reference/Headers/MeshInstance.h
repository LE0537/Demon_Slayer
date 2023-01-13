#pragma once

#include "VIBuffer_Instance.h"
#include "Model.h"

BEGIN(Engine)

/* 모델을 구성하는 각각의 메시다. */
class CMeshInstance final : public CVIBuffer_Instance
{
private:
	CMeshInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshInstance(const CMeshInstance& rhs);
	virtual ~CMeshInstance() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	void Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix, _uint iNumInstance);
	virtual HRESULT Initialize(void* pArg);
public:
	void Update(_float fTimeDelta);

public:
	HRESULT SetUp_Bones(class CModel* pModel);
	//HRESULT SetUp_Instance();

private:
	char						m_szName[MAX_PATH] = "";
	const aiMesh*				m_pAIMesh = nullptr;
	_uint						m_iMaterialIndex = 0;

	/* 이 메시에 영향을 주는 뼈들의 갯수 .*/
	_uint							m_iNumBones = 0;
	vector<class CHierarchyNode*>	m_Bones;

public:
	HRESULT Create_VertexBuffer_NonAnimModel(const aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Create_VertexBuffer_AnimModel(const aiMesh* pAIMesh, class CModel* pModel);
	//HRESULT Create_VertexBuffer_NonAnim_InstanceModel(const aiMesh* pAIMesh, _fmatrix PivotMatrix);


public:
	static CMeshInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"

CEffect::CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObj(rhs),
	m_EffectInfo(rhs.m_EffectInfo),
	m_Textures(rhs.m_Textures),
	m_Meshes(rhs.m_Meshes),
	m_Particle(rhs.m_Particle),
	m_TextureInfo(rhs.m_TextureInfo),
	m_MeshInfo(rhs.m_MeshInfo)
{
}

HRESULT CEffect::Initialize_Prototype(EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo
	, vector<CEffect_Mesh::MESH_INFO> MeshInfo)
{
	m_EffectInfo = EffectInfo;

	for (auto TexInfo : TextureInfo) {
		m_TextureInfo.push_back(TexInfo);
	}

	for (auto MeshInfo : MeshInfo) {
		m_MeshInfo.push_back(MeshInfo);
	}

	
	return S_OK;
}

HRESULT CEffect::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4((_float4*)pArg));

	_float3 vRadian;
	vRadian.x = XMConvertToRadians(m_EffectInfo.vRotation.x);
	vRadian.y = XMConvertToRadians(m_EffectInfo.vRotation.y);
	vRadian.z = XMConvertToRadians(m_EffectInfo.vRotation.z);

	m_pTransformCom->RotationAll(vRadian);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_EffectInfo.vPosition.x, m_EffectInfo.vPosition.y, m_EffectInfo.vPosition.z, 1.f));
	if (nullptr != pArg)
		m_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix() * (*(_matrix*)pArg));
	if (FAILED(Ready_Parts()))
		return E_FAIL;

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	m_fEffectTime += fTimeDelta;

	if (m_fEffectTime > m_EffectInfo.fEffectStartTime) {

		for (auto& pTex : m_Textures)
			pTex->Tick(fTimeDelta);

		for (auto& pMesh : m_Meshes)
			pMesh->Tick(fTimeDelta);

		if (m_fEffectTime > m_EffectInfo.fEffectStartTime + m_EffectInfo.fEffectLifeTime) {
			m_bDead = true;
			Set_Dead();

			for (auto& pTex : m_Textures)
				pTex->Set_Dead();

			for (auto& pMesh : m_Meshes)
				pMesh->Set_Dead();

			for (auto& pParticle : m_Particle)
				pParticle->Set_Dead();
		}
	}
}

void CEffect::Late_Tick(_float fTimeDelta)
{
	for (auto& pTex : m_Textures)
		pTex->Late_Tick(fTimeDelta);

	for (auto& pMesh : m_Meshes)
		pMesh->Late_Tick(fTimeDelta);
}

HRESULT CEffect::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Ready_Parts()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 咆胶贸 积己
	for (auto TexInfo : m_TextureInfo) {
		CGameObject*		pTexture = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_EffectTexture"));

		static_cast<CEffect_Texture*>(pTexture)->Set_Parents(this);
		static_cast<CEffect_Texture*>(pTexture)->Set_TexInfo(TexInfo);

		m_Textures.push_back((CEffect_Texture*)pTexture);
	}
	
	// 皋浆 积己
	for (auto MeshInfo : m_MeshInfo) {
		CGameObject*		pMesh = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_EffectMesh"));

		static_cast<CEffect_Mesh*>(pMesh)->Set_Parents(this);
		static_cast<CEffect_Mesh*>(pMesh)->Set_MeshInfo(MeshInfo);

		m_Meshes.push_back((CEffect_Mesh*)pMesh);
	}

	// 颇萍努 积己

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect * CEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo
	, vector<CEffect_Mesh::MESH_INFO> MeshInfo)
{
	CEffect*	pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(EffectInfo, TextureInfo, MeshInfo)))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	for (auto& pTex : m_Textures)
		Safe_Release(pTex);

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	for (auto& pParticle : m_Particle)
		Safe_Release(pParticle);

	Safe_Release(m_pTransformCom);
}

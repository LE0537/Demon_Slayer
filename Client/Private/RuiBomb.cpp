#include "stdafx.h"
#include "..\Public\RuiBomb.h"

#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "ImGuiManager.h"
CRuiBomb::CRuiBomb(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiBomb::CRuiBomb(const CRuiBomb & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiBomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiBomb::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_tBombInfo, pArg, sizeof(m_tBombInfo));

	

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tBombInfo.vPosition);
	
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_START, this);
	pEffectManger->Create_Effect(CEffect_Manager::EFF_BOOM1, this);

	RELEASE_INSTANCE(CEffect_Manager);
	_float4 vPosition; XMStoreFloat4(&vPosition, m_tBombInfo.vPosition);
	
//	CImGuiManager::Get_Instance()->Set_RuiBombPosition(vPosition);

	return S_OK;
}

void CRuiBomb::Tick(_float fTimeDelta)
{
	Set_EffectTime(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fDeadTime += fTimeDelta;

	if (m_fDeadTime > 5.f)
		Set_Dead();
}

void CRuiBomb::Late_Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CCollider*	pMyCollider = m_pOBBCom;
	CCollider*	pTargetCollider = m_tBombInfo.pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (0.2f <= m_fDeadTime && m_fDeadTime <= 0.6f)
	{
		if (pMyCollider->Collision(pTargetCollider) && !pGameInstance->Key_Pressing(DIK_T))
		{
			m_tBombInfo.pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-289.935f, 45.183f, -152.779f, 1.f));
		}
	}


	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiBomb::Render()
{
	return S_OK;
}

HRESULT CRuiBomb::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiBomb::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(7.5f, 5.f, 7.5f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiBomb * CRuiBomb::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiBomb*	pInstance = new CRuiBomb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiSphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiBomb::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiBomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiSphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiBomb::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}


#include "stdafx.h"
#include "..\Public\RuiDadBigStone.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDad.h"
CRuiDadBigStone::CRuiDadBigStone(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiDadBigStone::CRuiDadBigStone(const CRuiDadBigStone & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiDadBigStone::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiDadBigStone::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUIDADBIGSTONE));



	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_SHOTNET_MAIN, this);
	RELEASE_INSTANCE(CEffect_Manager);

	return S_OK;
}

void CRuiDadBigStone::Tick(_float fTimeDelta)
{
	if (m_fDeadTime < 2.f)
		Ready_Throw(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 4.f)
		Set_Dead();

}

void CRuiDadBigStone::Late_Tick(_float fTimeDelta)
{
	
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiDadBigStone::Render()
{
	return S_OK;
}

HRESULT CRuiDadBigStone::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiDadBigStone::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 50.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(8.f, 8.f, 8.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CRuiDadBigStone::Ready_Throw(_float fTimeDelta)
{
	CHierarchyNode*		pBody = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_BonePtr("C_Spine_3");
	CHierarchyNode*		pRightHand = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_BonePtr("R_Hand_1_Lct");
	CHierarchyNode*		pLeftHand = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_BonePtr("L_Hand_1_Lct");

	_vector BodyPos = pBody->Get_CombinedTransformationMatrix().r[3];
	_vector RightHandPos = pRightHand->Get_CombinedTransformationMatrix().r[3];
	_vector LeftHandPos = pLeftHand->Get_CombinedTransformationMatrix().r[3];

	_vector vPos = RightHandPos;
	_vector vLook = RightHandPos - LeftHandPos;

	_float fDist = XMVectorGetX(XMVector3Length(vLook));

	vPos -= XMVector3Normalize(vLook) * (fDist * 0.5f);

	_vector vPosLook = XMVector3Normalize(vPos - BodyPos);

	vPos += vPosLook * 2.f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CRuiDadBigStone::Throw(_float fTimeDelta)
{
}

void CRuiDadBigStone::Throw_End(_float fTimeDelta)
{
}



CRuiDadBigStone * CRuiDadBigStone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiDadBigStone*	pInstance = new CRuiDadBigStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiDadBigStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiDadBigStone::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiDadBigStone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiDadBigStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiDadBigStone::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

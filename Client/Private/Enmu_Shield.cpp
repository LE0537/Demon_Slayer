#include "stdafx.h"
#include "..\Public\Enmu_Shield.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "ImGuiManager.h"
#include "EnmuBoss.h"
CEnmu_Shield::CEnmu_Shield(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CEnmu_Shield::CEnmu_Shield(const CEnmu_Shield & rhs)
	: CCharacters(rhs)
{
}

HRESULT CEnmu_Shield::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnmu_Shield::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pHead = dynamic_cast<CCharacters*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Enmu_Head"))->Get_LayerFront());
	m_pTanjiro = dynamic_cast<CCharacters*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	m_pBattleTarget = m_pTanjiro;
	dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
	RELEASE_INSTANCE(CGameInstance);
	_vector vPos = { 0.956f, 16.6f, 174.106f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->Set_Scale(XMVectorSet(0.5f, 0.5f, 0.5f, 0.f));

	//CUI_Manager::Get_Instance()->Set_2P(this);


	CEnmuBoss::Get_Instance()->Add_EnmuParts(this);

	//CImGuiManager::Get_Instance()->Add_LiveCharacter(this);
	Set_Info();
	return S_OK;
}

void CEnmu_Shield::Tick(_float fTimeDelta)
{
	if (m_tInfo.iHp <= 0.f && m_fHealTime <= 6.f)
	{
		if (!m_bTarget)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(m_pHead);
			m_pTanjiro->Set_BattleTarget(m_pHead);
			RELEASE_INSTANCE(CGameInstance);
			m_bTarget = true;
			m_bFree = true;
		}
		m_fHealTime += fTimeDelta;
	}
	else if (m_fHealTime > 6.f)
	{

		m_bFree = false;
		m_tInfo.iHp = m_tInfo.iMaxHp;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		m_pTanjiro->Set_BattleTarget(this);
		RELEASE_INSTANCE(CGameInstance);
		m_bTarget = false;
		m_fHealTime = 0.f;
	}
	//m_pModelCom->Play_Animation(fTimeDelta);

	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("Root");
	if (nullptr == pSocket)
		return;
	_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

	m_pSphereCom->Update(matColl);

	HandleInput();
	TickState(fTimeDelta);





}

void CEnmu_Shield::Late_Tick(_float fTimeDelta)
{

	LateTickState(fTimeDelta);
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}

	if (g_bCollBox)
	{
		m_pRendererCom->Add_CollBox(m_pSphereCom);
	}


}

HRESULT CEnmu_Shield::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CEnmu_Shield::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (m_bShadowAlphaIncrease == false)
		m_ShadowMatrix = m_pTransformCom->Get_WorldMatrix();

	if (m_bShadowAlphaDecrease == true)
		m_ShadowMatrix = m_pTransformCom->Get_WorldMatrix();


	_float4 vTemp = *(_float4*)&m_ShadowMatrix.r[3];

	if (m_bShadowAlphaIncrease == true)
	{
		vTemp.x += 100.f;
		vTemp.y += 100.f;
		vTemp.z += 100.f;
	}

	*(_float4*)&m_ShadowMatrix.r[3] = vTemp;

	//_float4x4 WorldMatrix = m_pTransformCom->Get_World4x4();

	_float4x4	TransposeMatrix;
	XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(m_ShadowMatrix));


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &TransposeMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;

	vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
	vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
	vLightUp = { 0.f, 1.f, 0.f ,0.f };
	matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

void CEnmu_Shield::HandleInput()
{

}

void CEnmu_Shield::TickState(_float fTimeDelta)
{
	if (m_bHit == true)
	{
		if (m_pModelCom->Get_End(6))
		{
			m_pModelCom->Set_End(6);

			m_iAnimIndex = 2;
			m_pModelCom->Set_CurrentAnimIndex(2);
			m_pModelCom->Set_Loop(2, true);
			m_pModelCom->Set_LinearTime(2, 0.1f);

			Set_GodMode(false);

			m_bHit = false;
		}
	}
	
	m_iState = CEnmuBoss::Get_Instance()->Get_BossState();

}

void CEnmu_Shield::LateTickState(_float fTimeDelta)
{

}

HRESULT CEnmu_Shield::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEnmu_Shield::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Shield"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(400.f, 400.f, 400.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;



	return S_OK;
}

void CEnmu_Shield::Set_Info()
{
	m_tInfo.strName = TEXT("����(����)");
	m_tInfo.bOni = true;
	m_tInfo.iMaxHp = 200;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iUnicCount = 0;
	m_tInfo.iDmg = 20;
	m_tInfo.iCombo = 0;
	m_tInfo.fComboTime = 0.f;
	m_tInfo.fPowerUp = 1.f;
	m_tInfo.fPowerUpTime = 0.f;
	m_tInfo.iFriendMaxBar = 1000;
	m_tInfo.iFriendBar = m_tInfo.iFriendMaxBar;
	m_tInfo.bGuard = false;
	m_tInfo.bChange = false;
	m_tInfo.iMaxGuard = 500;
	m_tInfo.iGuard = m_tInfo.iMaxGuard;
}

void CEnmu_Shield::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if (Get_GodMode() == false)
	{
		m_pModelCom->Reset_Anim(6);

		m_iAnimIndex = 6;
		m_pModelCom->Set_CurrentAnimIndex(6);
		m_pModelCom->Set_Loop(6);
		m_pModelCom->Set_LinearTime(6, 0.1f);
		m_bHit = true;
	}

}

void CEnmu_Shield::Get_GuardHit(_int eType)
{

	

}

void CEnmu_Shield::Player_TakeDown(_float _fPow, _bool _bJump)
{
	if (Get_GodMode() == false)
	{
		m_pModelCom->Reset_Anim(6);

		m_iAnimIndex = 6;
		m_pModelCom->Set_CurrentAnimIndex(6);
		m_pModelCom->Set_Loop(6);
		m_pModelCom->Set_LinearTime(6, 0.1f);
		m_bHit = true;
	}

}

void CEnmu_Shield::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	if (Get_GodMode() == false)
	{
		m_pModelCom->Reset_Anim(6);
		m_iAnimIndex = 6;
		m_pModelCom->Set_CurrentAnimIndex(6);
		m_pModelCom->Set_Loop(6);
		m_pModelCom->Set_LinearTime(6, 0.1f);
		m_bHit = true;
	}
	
}

void CEnmu_Shield::Play_Scene()
{
}

CEnmu_Shield * CEnmu_Shield::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnmu_Shield*	pInstance = new CEnmu_Shield(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEnmu_Shield"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnmu_Shield::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEnmu_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEnmu_Shield"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnmu_Shield::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);

}

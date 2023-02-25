#include "stdafx.h"
#include "..\Public\Enmu.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "ImGuiManager.h"
#include "EnmuToolState.h"
#include "EnmuIdleState.h"
#include "EnmuGuardHitState.h"
#include "EnmuGuardState.h"


#include "EnmuHitState.h"
#include "EnmuUpperHitState.h"
#include "EnmuTakeDownState.h"
using namespace Enmu;

CEnmu::CEnmu(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CEnmu::CEnmu(const CEnmu & rhs)
	: CCharacters(rhs)
{
}

HRESULT CEnmu::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnmu::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	if (m_i1p == 10)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVAKAZA, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		RELEASE_INSTANCE(CGameInstance);

		_vector vPos = { -0.287479f,5.701f,196.877f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		m_pNavigationCom->Find_CurrentCellIndex(vPos);

		m_tInfo.bSub = false;
		m_bChange = false;
		//CUI_Manager::Get_Instance()->Set_2P(this);
	}
	else if (m_i1p == 11)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		//dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		RELEASE_INSTANCE(CGameInstance);
		_vector vPos = { 64.f, 0.f, 38.5f,1.f };
		if (g_iLevel == LEVEL_BATTLEENMU)
			vPos = { -0.302f, 16.420f, 192.321f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		m_pNavigationCom->Find_CurrentCellIndex(vPos);

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		CUI_Manager::Get_Instance()->Set_2P(this);
	}

	Set_Info();
	CEnmuState* pState = new CIdleState();
	m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pState);

	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);

	return S_OK;
}

void CEnmu::Tick(_float fTimeDelta)
{
	if (m_i1p == 10)
	{
		HandleInput();
		TickState(fTimeDelta);

	}
	else if (m_i1p == 11)
	{
		if (!m_tInfo.bSub && m_bBattleStart)
		{
			CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
			if (nullptr == pSocket)
				return;
			_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

			m_pSphereCom->Update(matColl);

			HandleInput();
			TickState(fTimeDelta);

			m_bAiMode = true;
		}
	}

	//if (m_pTransformCom->Get_Jump() == true)
	//	m_tInfo.bJump = true;
	//else if (m_pTransformCom->Get_Jump() == false)
	//	m_tInfo.bJump = false;

}

void CEnmu::Late_Tick(_float fTimeDelta)
{

	if (m_i1p == 10)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));
		LateTickState(fTimeDelta);
		//if (fDist < 5.f)
		//{
		//	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
		//	pUIManager->Set_EnmuBattle(true);
		//	RELEASE_INSTANCE(CUI_Manager);
		//}
		if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
		{
			if (fDist < 45.f)
			{

				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			}
		}
		RELEASE_INSTANCE(CGameInstance);
	}
	else if (m_i1p == 11)
	{
		if (!m_tInfo.bSub && m_bBattleStart)
		{

			LateTickState(fTimeDelta);
			if (m_bRender)
			{
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			}

			if (g_bCollBox)
			{
				m_pRendererCom->Add_Debug(m_pSphereCom);
			}

		}
	}

}

HRESULT CEnmu::Render()
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

HRESULT CEnmu::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);



	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;




	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;
	if (g_iLevel == LEVEL_BATTLEENMU)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else if (g_iLevel == LEVEL_ADVAKAZA)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
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

void CEnmu::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CEnmuState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CEnmuState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pState);
}

void CEnmu::HandleInput()
{
	CEnmuState* pNewState = m_pEnmuState->HandleInput(this);

	if (pNewState)
		m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pNewState);
}

void CEnmu::TickState(_float fTimeDelta)
{
	CEnmuState* pNewState = m_pEnmuState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pNewState);
}

void CEnmu::LateTickState(_float fTimeDelta)
{
	CEnmuState* pNewState = m_pEnmuState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pNewState);
}

HRESULT CEnmu::SetUp_ShaderResources()
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

HRESULT CEnmu::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Enmu"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(160.f, 160.f, 160.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (m_i1p == 11)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainBattle"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainNavi"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}

	return S_OK;
}

void CEnmu::Set_Info()
{
	m_tInfo.strName = TEXT("¿£¹«");
	m_tInfo.bOni = true;
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iUnicCount = 0;
	m_tInfo.iDmg = 15;
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

void CEnmu::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	CEnmuState* pState = new CHitState(_fPow, _bJumpHit);
	m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pState);

}

void CEnmu::Get_GuardHit(_int eType)
{
	CEnmuState* pState;
	if (eType == CEnmuState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CEnmu::ANIMID::ANIM_GUARDHIT_0);
		pState = new CGuardHitState(CEnmuState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CEnmu::ANIMID::ANIM_GUARDHIT_0);
		pState = new CGuardHitState(CEnmuState::STATE_TYPE::TYPE_LOOP);
	}
	m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pState);
}

void CEnmu::Player_TakeDown(_float _fPow, _bool _bJump)
{
	CEnmuState* pState = new CTakeDownState(_fPow, _bJump);
	m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pState);

}

void CEnmu::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	CEnmuState* pState = new CUpperHitState(eHitType, CEnmuState::STATE_TYPE::TYPE_START, fBoundPower, fJumpPower, fKnockBackPower);
	m_pEnmuState = m_pEnmuState->ChangeState(this, m_pEnmuState, pState);

}

void CEnmu::Play_Scene()
{
}

CEnmu * CEnmu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnmu*	pInstance = new CEnmu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEnmu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnmu::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEnmu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEnmu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnmu::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pNavigationCom);

	Safe_Delete(m_pEnmuState);
}

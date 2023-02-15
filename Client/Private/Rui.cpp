#include "stdafx.h"
#include "..\Public\Rui.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "ImGuiManager.h"
#include "RuiState.h"
#include "RuiIdleState.h"
#include "RuiToolState.h"

#include "Level_GamePlay.h"

#include "AiState.h"
#include "RuiHitState.h"
#include "RuiBattleSTState.h"
#include "RuiGuardHitState.h"
#include "Effect_Manager.h"
#include "RuiTakeDownState.h"
#include "RuiUpperHitState.h"
using namespace Rui;


CRui::CRui(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CRui::CRui(const CRui & rhs)
	: CCharacters(rhs)
{
}

HRESULT CRui::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRui::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_i1p = 11;

	if (m_i1p != 10 && m_i1p != 11)
	{
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
		m_pNavigationCom->Set_NaviIndex(tCharacterDesc.iNaviIndex);

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		if (!m_tInfo.bSub)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			*(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar) = this;
			if (m_i1p == 1)
			{
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);

				CUI_Manager::Get_Instance()->Set_1P(this);
			}
			else if (m_i1p == 2)
			{
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);

				CUI_Manager::Get_Instance()->Set_2P(this);
			}
			else if (m_i1p == 10)
			{
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);

				CUI_Manager::Get_Instance()->Set_2P(this);
			}
	
			RELEASE_INSTANCE(CGameInstance);

		}
		else
		{
			m_pSubChar = *(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar);
			m_pSubChar->Set_SubChar(this);


		}
	}
	else if (m_i1p == 10)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		RELEASE_INSTANCE(CGameInstance);
		_vector vPos = { -860.374f,92.52f,-68.017f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	
		m_pNavigationCom->Find_CurrentCellIndex(vPos);

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
	//	CUI_Manager::Get_Instance()->Set_2P(this);

	}

	else if (m_i1p == 11)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		RELEASE_INSTANCE(CGameInstance);
		_vector vPos = { 64.f, 0.f, 38.5f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		m_pNavigationCom->Find_CurrentCellIndex(vPos);

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		CUI_Manager::Get_Instance()->Set_2P(this);

	}


	CRuiState* pState = new CIdleState();
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);

	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);

	Set_Info();
	return S_OK;
}

void CRui::Tick(_float fTimeDelta)
{
	if (!m_tInfo.bSub)
	{
		
		if (m_bBattleStart)
		{
			CRuiState* pState = new CBattleStartState();
			m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
			m_bBattleStart = false;
		}
		

		if (m_i1p == 11)
		{
			if (m_bStart == true)
			{
				CRuiState* pState = new CIdleState();
				m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
				m_bStart = false;
				m_bAiState = true;
			}

			m_tInfo.iSkBar += 1000;
			Boss_Tick(fTimeDelta);
		}
		else
		{
			m_fDelta = fTimeDelta;
			if (m_tInfo.fHitTime > 0.f)
				m_tInfo.fHitTime -= fTimeDelta;
			if (m_tInfo.fGuardTime > 0.f)
				m_tInfo.fGuardTime -= fTimeDelta;
			if (m_tInfo.fPowerUpTime > 0.f)
			{
				m_tInfo.fPowerUpTime -= fTimeDelta;
				if (m_tInfo.fPowerUpTime <= 0.f)
				{
					m_tInfo.fPowerUp = 1.f;
					m_tInfo.iPowerIndex = 0;
				}
			}
			if (m_tInfo.iPowerIndex == 2)
				m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
			if (m_tInfo.fHitTime <= 0.f && !m_tInfo.bSub)
				HandleInput(fTimeDelta);
		}
		TickState(fTimeDelta);


		CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
		if (nullptr == pSocket)
			return;
		_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

		m_pSphereCom->Update(matColl);


	if (m_pRuiState->Get_RuiState() == CRuiState::STATE_JUMP || m_pRuiState->Get_RuiState() == CRuiState::STATE_CHANGE || m_pRuiState->Get_RuiState() == CRuiState::STATE_JUMP_ATTACK)
		m_tInfo.bJump = true;
	else
		m_tInfo.bJump = false;

	}
	if (m_pTransformCom->Get_Jump() == true)
		m_tInfo.bJump = true;
	if (m_pRuiState != nullptr)
	{
		m_iState = m_pRuiState->Get_RuiState();
	}

}

void CRui::Late_Tick(_float fTimeDelta)
{
	if (!m_tInfo.bSub)
	{
		LateTickState(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		if (g_bCollBox)
		{
			m_pRendererCom->Add_Debug(m_pSphereCom);
		}
	}
}

HRESULT CRui::Render()
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

		//aiTextureType_AMBIENT
	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}
HRESULT CRui::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;
	if (g_iLevel == 1)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else if (g_iLevel == 2)
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
HRESULT CRui::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Rui"), (CComponent**)&m_pModelCom)))
		return E_FAIL;




	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(130.f, 130.f, 130.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (m_i1p == 10)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}


	return S_OK;
}

void CRui::Boss_Tick(_float fTimeDelta)
{
	CAiState* pNewState = (CAiState*)m_pRuiState->HandleInput(this);

	if (pNewState)
	{
		m_AIStateList.push_back(pNewState);
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);
	}
	
}

void CRui::Boss_LateTick(_float fTimeDelta)
{


}

void CRui::Boss_Render()
{
}



void CRui::Set_Info()
{
	m_tInfo.strName = TEXT("∑Á¿Ã");
	m_tInfo.bOni = true;
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iUnicCount = 0;
	m_tInfo.iDmg = 10;
	m_tInfo.iCombo = 0;
	m_tInfo.fComboTime = 0.f;
	m_tInfo.fPowerUp = 1.f;
	m_tInfo.fPowerUpTime = 0.f;
	m_tInfo.iPowerIndex = 0;
	m_tInfo.iFriendMaxBar = 1000;
	m_tInfo.iFriendBar = m_tInfo.iFriendMaxBar;
	m_tInfo.bGuard = false;
	m_tInfo.bChange = false;
	m_tInfo.iMaxGuard = 500;
	m_tInfo.iGuard = m_tInfo.iMaxGuard;
}

_bool CRui::Get_RuiHit()
{
	return m_bIsHit;
}

void CRui::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CRuiState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CRuiState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
}

void CRui::HandleInput(_float fTimeDelta)
{
	CRuiState* pNewState = m_pRuiState->HandleInput(this);

	if (pNewState)
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);

}


void CRui::TickState(_float fTimeDelta)
{
	CRuiState* pNewState = m_pRuiState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);
}

void CRui::LateTickState(_float fTimeDelta)
{
	CRuiState* pNewState = m_pRuiState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);
	if (m_pRuiState->Get_RuiState() == CRuiState::STATE_MOVE)
		m_fEffectTime += fTimeDelta;
	if (m_fEffectTime > 0.3f)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUN, this);

		RELEASE_INSTANCE(CEffect_Manager);
		m_fEffectTime = 0.f;
	}
}

void CRui::Take_Damage(_float _fPow, _bool _bJumpHit)
{

	if (m_pRuiState->Get_RuiState() == CRuiState::STATE_HIT)
		m_pModelCom->Reset_Anim(CRui::ANIMID::ANIM_HIT);

	CRuiState* pState = new CHitState(_fPow, _bJumpHit);
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);

}

void CRui::Get_GuardHit(_int eType)
{
	CRuiState* pState;
	if (eType == CRuiState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CRui::ANIMID::ANIM_GUARD_HIT_0);
		pState = new CGuardHitState(CRuiState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CRui::ANIMID::ANIM_GUARD_HIT_1);
		pState = new CGuardHitState(CRuiState::STATE_TYPE::TYPE_LOOP);
	}
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
}

void CRui::Player_TakeDown(_float _fPow, _bool _bJump)
{
	CRuiState* pState = new CTakeDownState(_fPow, _bJump);
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
}

void CRui::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	CRuiState* pState = new CUpperHitState(eHitType, CRuiState::STATE_TYPE::TYPE_START, fBoundPower, fJumpPower, fKnockBackPower);
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
}

HRESULT CRui::SetUp_ShaderResources()
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




CRui * CRui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRui*	pInstance = new CRui(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRui"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRui::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRui(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRui"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRui::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pNavigationCom);

	Safe_Delete(m_pRuiState);

	//for (auto & iter : m_AIStateList)
	//	Safe_Delete(iter);

	//m_AIStateList.clear();
	
}

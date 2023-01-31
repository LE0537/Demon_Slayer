#include "stdafx.h"
#include "..\Public\Nezuko.h"

#include "Layer.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "ImGuiManager.h"
#include "Level_GamePlay.h"
#include "NezukoState.h"
#include "NezukoIdleState.h"
#include "NezukoToolState.h"
#include "NezukoHitState.h"

using namespace Nezuko;

CNezuko::CNezuko(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CNezuko::CNezuko(const CNezuko & rhs)
	: CCharacters(rhs)
{
}

HRESULT CNezuko::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNezuko::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	m_pNavigationCom->Set_NaviIndex(tCharacterDesc.iNaviIndex);

	Set_Info();
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
		RELEASE_INSTANCE(CGameInstance);
	}
	else
	{
		m_pSubChar = *(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar);
		m_pSubChar->Set_SubChar(this);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
		m_bChange = true;
		if (m_i1p == 1)
			CUI_Manager::Get_Instance()->Set_1P_2(this);
		else if (m_i1p == 2)
			CUI_Manager::Get_Instance()->Set_2P_2(this);
	}



	CNezukoState* pState = new CIdleState();
	m_pNezukoState = m_pNezukoState->ChangeState(this, m_pNezukoState, pState);


	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);
	return S_OK;
}

void CNezuko::Tick(_float fTimeDelta)
{
	if (!m_bChange)
	{
		__super::Tick(fTimeDelta);
		m_fDelta = fTimeDelta;
		if (m_tInfo.fHitTime > 0.f)
			m_tInfo.fHitTime -= fTimeDelta;
		if (m_fChangeDelay > 0.f)
			m_fChangeDelay -= fTimeDelta;
		if (m_tInfo.fHitTime <= 0.f && !m_tInfo.bSub)
			HandleInput();

		TickState(fTimeDelta);

		CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
		if (nullptr == pSocket)
			return;
		_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

		m_pSphereCom->Update(matColl);

	}
	if (m_pNezukoState->Get_NezukoState() == CNezukoState::STATE_JUMP || m_pNezukoState->Get_NezukoState() == CNezukoState::STATE_CHANGE 
		|| m_pNezukoState->Get_NezukoState() == CNezukoState::STATE_SKILL_FALLCUT || m_pNezukoState->Get_NezukoState() == CNezukoState::STATE_SKILL_MOVE)
		m_tInfo.bJump = true;
	else
		m_tInfo.bJump = false;
}

void CNezuko::Late_Tick(_float fTimeDelta)
{
	if (!m_bChange)
	{
		LateTickState(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		//m_pModelCom->Play_Animation(fTimeDelta);
		if (g_bCollBox)
		{
			m_pRendererCom->Add_Debug(m_pSphereCom);
		}
	}
}

HRESULT CNezuko::Render()
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

	if (!m_tInfo.bChange && m_fChangeDelay <= 0.f)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] += 15.f;
		switch (m_i1p)
		{
		case 1:
			if (m_tInfo.iFriendBar >= 100 && pGameInstance->Key_Pressing(DIK_U))
			{
				m_fChangeTime += m_fDelta;
				if (m_fChangeTime > 0.5f)
				{
					m_tInfo.iFriendBar -= 100;
					m_tInfo.bSub = true;
					CUI_Manager::Get_Instance()->Set_1P(m_pSubChar);
					CUI_Manager::Get_Instance()->Set_1P_2(this);
					m_pSubChar->Set_ChangeDelay(1.f);
					m_pSubChar->Set_Sub(false);
					m_pSubChar->Change_Info(m_tInfo);
					m_pSubChar->Set_ChangeInfo(true);
					m_pSubChar->Set_Change(false, vPos);
					m_pSubChar->Set_BattleTarget(m_pBattleTarget);
					m_pBattleTarget->Set_BattleTarget(m_pSubChar);
					m_fChangeTime = 0.f;
				}
			}
			else
			{
				m_fChangeTime = 0.f;
			}
			break;
		case 2:
			if (m_tInfo.iFriendBar >= 100 && pGameInstance->Key_Pressing(DIK_V))
			{
				m_fChangeTime += m_fDelta;
				if (m_fChangeTime > 0.5f)
				{
					m_tInfo.iFriendBar -= 100;
					m_tInfo.bSub = true;
					CUI_Manager::Get_Instance()->Set_2P(m_pSubChar);
					CUI_Manager::Get_Instance()->Set_2P_2(this);
					m_pSubChar->Set_ChangeDelay(1.f);
					m_pSubChar->Set_Sub(false);
					m_pSubChar->Change_Info(m_tInfo);
					m_pSubChar->Set_ChangeInfo(true);
					m_pSubChar->Set_Change(false, vPos);
					m_pSubChar->Set_BattleTarget(m_pBattleTarget);
					m_pBattleTarget->Set_BattleTarget(m_pSubChar);
					m_fChangeTime = 0.f;
				}
			}
			else
			{
				m_fChangeTime = 0.f;
			}
			break;
		default:
			break;
		}
	}
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CNezuko::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
	_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
	_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
	_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

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

void CNezuko::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CNezukoState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CNezukoState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pNezukoState = m_pNezukoState->ChangeState(this, m_pNezukoState, pState);
}

void CNezuko::HandleInput()
{
	CNezukoState* pNewState = m_pNezukoState->HandleInput(this);

	if (pNewState)
		m_pNezukoState = m_pNezukoState->ChangeState(this, m_pNezukoState, pNewState);
}

void CNezuko::TickState(_float fTimeDelta)
{
	CNezukoState* pNewState = m_pNezukoState->Tick(this,fTimeDelta);

	if (pNewState)
		m_pNezukoState = m_pNezukoState->ChangeState(this, m_pNezukoState, pNewState);
}

void CNezuko::LateTickState(_float fTimeDelta)
{
	CNezukoState* pNewState = m_pNezukoState->Late_Tick(this,fTimeDelta);

	if (pNewState)
		m_pNezukoState = m_pNezukoState->ChangeState(this, m_pNezukoState, pNewState);
}

HRESULT CNezuko::SetUp_ShaderResources()
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

HRESULT CNezuko::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Nezuko"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(100.f, 100.f, 100.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	return S_OK;
}

void CNezuko::Set_Info()
{
	m_tInfo.strName = TEXT("³×ÁîÄÚ");
	m_tInfo.bOni = false;
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iDmg = 10;
	m_tInfo.iCombo = 0;
	m_tInfo.fComboTime = 0.f;
	m_tInfo.bPowerUp = false;
	m_tInfo.fPowerUpTime = 0.f;
	m_tInfo.iFriendMaxBar = 1000;
	m_tInfo.iFriendBar = 0;
	m_tInfo.bGuard = false;
	m_tInfo.bChange = false;
}

void CNezuko::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if (m_pNezukoState->Get_NezukoState() == CNezukoState::STATE_HIT)
		m_pModelCom->Reset_Anim(CNezuko::ANIMID::ANIM_HIT);

	CNezukoState* pState = new CHitState(_fPow, _bJumpHit);
	m_pNezukoState = m_pNezukoState->ChangeState(this, m_pNezukoState, pState);

}

void CNezuko::Get_GuardHit(_int eType)
{
}

CNezuko * CNezuko::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNezuko*	pInstance = new CNezuko(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CNezuko"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNezuko::Clone(void * pArg)
{
	CGameObject*	pInstance = new CNezuko(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNezuko"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNezuko::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pNavigationCom);
	Safe_Delete(m_pNezukoState);

}

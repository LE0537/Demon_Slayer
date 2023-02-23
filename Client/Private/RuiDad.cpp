#include "stdafx.h"
#include "..\Public\RuiDad.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "RuiDadIdleState.h"
#include "ImGuiManager.h"
#include "Tanjiro.h"
#include "RuiDadHitState.h"
#include "RuiDadGuardHitState.h"
#include "AngryState.h"
using namespace RuiDad;

CRuiDad::CRuiDad(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CRuiDad::CRuiDad(const CRuiDad & rhs)
	: CCharacters(rhs)
{
}

HRESULT CRuiDad::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiDad::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_i1p == 10)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		RELEASE_INSTANCE(CGameInstance);

		//	_vector vPos = { -860.374f,92.52f,-68.017f,1.f }; 보스자리
		_vector vPos = { -335.479f,42.501f,-328.243f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		m_pNavigationCom->Find_CurrentCellIndex(vPos);
		 
		m_tInfo.bSub = false;
		m_bChange = false;
		//CUI_Manager::Get_Instance()->Set_2P(this);
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

	CRuiDadState* pState = new CAngryState();
	m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pState);

	//CImGuiManager::Get_Instance()->Add_LiveCharacter(this);
	Set_Info();
	return S_OK;
}

void CRuiDad::Tick(_float fTimeDelta)
{
	if (m_i1p == 10)
	{
		HandleInput();
		TickState(fTimeDelta);

		if (dynamic_cast<CTanjiro*>(m_pBattleTarget)->Get_Quest2())
		{
			if (!m_bQuestStart)
			{
				m_bQuestStart = true;
				m_pTransformCom->Set_PlayerLookAt(m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			}
			_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist <= 9.f) // idle
			{
				m_bQuestStop = true;
				dynamic_cast<CTanjiro*>(m_pBattleTarget)->Set_Stop(false);
			}
		
		}
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

	m_iState = m_pRuiDadState->Get_RuiDadState();

}

void CRuiDad::Late_Tick(_float fTimeDelta)
{
	
	if (m_i1p == 10)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));
		LateTickState(fTimeDelta);
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

HRESULT CRuiDad::Render()
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

HRESULT CRuiDad::Render_ShadowDepth()
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
	if (g_iLevel == LEVEL_GAMEPLAY)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else if (g_iLevel == LEVEL_ADVRUI)
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

void CRuiDad::HandleInput()
{
	CRuiDadState* pNewState = m_pRuiDadState->HandleInput(this);

	if (pNewState)
		m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pNewState);
}

void CRuiDad::TickState(_float fTimeDelta)
{
	CRuiDadState* pNewState = m_pRuiDadState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pNewState);
}

void CRuiDad::LateTickState(_float fTimeDelta)
{
	CRuiDadState* pNewState = m_pRuiDadState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pNewState);
}

HRESULT CRuiDad::SetUp_ShaderResources()
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

HRESULT CRuiDad::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("RuiDad"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(160.f, 160.f, 160.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (m_i1p == 11)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}

	return S_OK;
}

void CRuiDad::Set_Info()
{
	m_tInfo.strName = TEXT("아빠 거미");
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
	m_tInfo.iFriendMaxBar = 1000;
	m_tInfo.iFriendBar = m_tInfo.iFriendMaxBar;
	m_tInfo.bGuard = false;
	m_tInfo.bChange = false;
	m_tInfo.iMaxGuard = 500;
	m_tInfo.iGuard = m_tInfo.iMaxGuard;
}

void CRuiDad::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if (m_pRuiDadState->Get_RuiDadState() == 18)
	{
		return;
	}


	CRuiDadState* pState = new CHitState(_fPow, CRuiDadState::STATE_TYPE::TYPE_START, false);
	m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pState);
}

void CRuiDad::Get_GuardHit(_int eType)
{
	CRuiDadState* pState;
	if (eType == CRuiDadState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CRuiDad::ANIMID::ANIM_GUARDHIT_0);
		pState = new CGuardHitState(CRuiDadState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CRuiDad::ANIMID::ANIM_GUARDHIT_0);
		pState = new CGuardHitState(CRuiDadState::STATE_TYPE::TYPE_LOOP);
	}
	m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pState);
}

void CRuiDad::Player_TakeDown(_float _fPow, _bool _bJump)
{
	if (m_pRuiDadState->Get_RuiDadState() == 18)
	{
		return;
	}

	CRuiDadState* pState = new CHitState(0.f, CRuiDadState::STATE_TYPE::TYPE_START, false);
	m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pState);

}

void CRuiDad::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	if (m_pRuiDadState->Get_RuiDadState() == 18)
	{
		return;
	}

	CRuiDadState* pState = new CHitState(0.f, CRuiDadState::STATE_TYPE::TYPE_START, false);
	m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pState);

}

void CRuiDad::Play_Scene()
{
}

CRuiDad * CRuiDad::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiDad*	pInstance = new CRuiDad(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiDad"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRuiDad::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiDad(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiDad"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiDad::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pNavigationCom);

	Safe_Delete(m_pRuiDadState);
}

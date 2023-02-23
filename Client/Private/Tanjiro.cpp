#include "stdafx.h"
#include "..\Public\Tanjiro.h"
#include "Layer.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
// state
#include "TanjiroState.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
#include "TanjiroHitState.h"
#include "TanjiroWeapon.h"
#include "TanjiroSheath.h"
#include "TanjiroGuardHitState.h"
#include "TanjiroToolState.h"
#include "ImGuiManager.h"
#include "Level_GamePlay.h"
#include "TanjiroBattleSTState.h"
#include "Effect_Manager.h"
#include "TanjiroTakeDownState.h"
#include "TanjiroUpperHitState.h"

#include "HinoCami_CinemaState.h"

// 오의히트
#include "HitCinema_Rui.h"



#include "Level_Loading.h"

using namespace Tanjiro;


CTanjiro::CTanjiro(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CTanjiro::CTanjiro(const CTanjiro & rhs)
	: CCharacters(rhs)
{
}

HRESULT CTanjiro::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTanjiro::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);
	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	if (FAILED(Ready_Parts2()))
		return E_FAIL;

	if (m_i1p != 10 && m_i1p != 20)
	{
		if (g_iLevel == LEVEL_BATTLEENMU)
		{
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-0.435f,16.413f,212.616f,1.f));
			m_pNavigationCom->Find_CurrentCellIndex(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
		}
		else
		{
			m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
			m_pNavigationCom->Set_NaviIndex(tCharacterDesc.iNaviIndex);
		}

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		if (!m_tInfo.bSub)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			*(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar) = this;
			if (m_i1p == 1)
			{
				if (g_iLevel == LEVEL_BATTLEENMU)
					dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);
				else
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
	}
	else if (m_i1p == 10)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_CamType(true);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);
		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		_vector vPos;
		if (CUI_Manager::Get_Instance()->Get_SaveStory())
		{
			vPos = { -278.321f,38.569f,-329.935f,1.f };
			m_pTransformCom->Set_Rotation(_float3(0.f, 220.f, 0.f));
		}
		else
		{
			vPos = { -100.f,3.204f,8.337f,1.f };
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		}
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		m_pNavigationCom->Find_CurrentCellIndex(vPos);
		*(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar) = this;
		CUI_Manager::Get_Instance()->Set_1P(this);
		m_bStoryKey = true;
		RELEASE_INSTANCE(CGameInstance);
		m_i1p = 1;
	}
	else if (m_i1p == 20)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVAKAZA, TEXT("Layer_Camera"))->Get_LayerFront())->Set_CamType(true);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVAKAZA, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);
		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		
		_vector vPos = { 3.533f,5.701f,403.538f,1.f };
	
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

		m_pNavigationCom->Find_CurrentCellIndex(vPos);
		*(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar) = this;
		CUI_Manager::Get_Instance()->Set_1P(this);
		m_bStoryKey = true;
		RELEASE_INSTANCE(CGameInstance);
		m_i1p = 1;
	}
	Set_Info();
	CTanjiroState* pState = new CIdleState();
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);


	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);


	m_ePlayerType = CCharacters::PLAYER_TYPE::PLAYER_TANJIRO;


	return S_OK;
}

void CTanjiro::Tick(_float fTimeDelta)
{
	if (!m_bChange)
	{
		__super::Tick(fTimeDelta);

		if (m_bBattleStart)
		{
			CTanjiroState* pState = new CBattleStartState();
			m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
			m_bBattleStart = false;
		}


		m_fDelta = fTimeDelta;
		if (m_tInfo.fHitTime > 0.f)
			m_tInfo.fHitTime -= fTimeDelta;
		if (m_fChangeDelay > 0.f)
			m_fChangeDelay -= fTimeDelta;
		if (m_tInfo.fGuardTime > 0.f)
			m_tInfo.fGuardTime -= fTimeDelta;
		if (m_tInfo.fPowerUpTime > 0.f)
		{
			m_tInfo.fPowerUpTime -= fTimeDelta;
			if (m_tInfo.fPowerUpTime <= 0.f)
			{
				m_tInfo.fPowerUp = 1.f;
				m_tInfo.iPowerIndex = 0;
				m_bIsKagura = false;
			}
		}
		if (m_tInfo.iPowerIndex == 2)
			m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
		if (m_tInfo.fHitTime <= 0.f && !m_tInfo.bSub && !m_bStop)
			HandleInput(fTimeDelta);

		TickState(fTimeDelta);

		CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
		if (nullptr == pSocket)
			return;
		_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());



		m_pSphereCom->Update(matColl);

		if (g_iLevel == LEVEL_ADVRUI)
		{
			Set_Shadow();
			Check_QuestEvent(fTimeDelta);
		}
		else if (g_iLevel == LEVEL_ADVAKAZA)
			Set_Shadow();
	}

	if (m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_JUMP
		|| m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_CHANGE ||
		m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_JUMP_ATTACK || m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_SKILL_KAGURA_COMMON)
	{
		m_tInfo.bJump = true;
	}
	else
	{ 
		m_tInfo.bJump = false;
	}

	if (m_pTransformCom->Get_Jump() == true)
		m_tInfo.bJump = true;


	if (m_pTanjiroState != nullptr)
	{
		m_iState = m_pTanjiroState->Get_TanjiroState();
	}
}




void CTanjiro::Late_Tick(_float fTimeDelta)
{
	if (!m_bChange)
	{
		LateTickState(fTimeDelta);

		m_pWeapon->Tick(fTimeDelta);
		m_pSheath->Tick(fTimeDelta);

		if (!m_bRender)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

			dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Set_Render(true);
			dynamic_cast<CTanjiroSheath*>(m_pSheath)->Set_Render(true);

			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pSheath);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pSheath);
		}

		if (g_bCollBox)
		{
			m_pRendererCom->Add_Debug(m_pSphereCom);
		}


	}
}

HRESULT CTanjiro::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//m_pNavigationCom->Render();

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (i == 0 || i == 1)
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_MaskTexture", i, aiTextureType_NORMALS)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
	}
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		if (!m_tInfo.bChange && m_fChangeDelay <= 0.f && vPos.m128_f32[1] <= m_pNavigationCom->Get_NavigationHeight().y
			&& -50000.f == XMVectorGetX(m_pSubChar->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
			vPos.m128_f32[1] += 15.f;
			_vector vLastPos = vPos;
			//	vPos -= XMVector3Normalize(vLook) * 5.f;
			vPos -= XMVector3Normalize(vRight) * 5.f;
			switch (m_i1p)
			{
			case 1:
				if (m_tInfo.iFriendBar >= 500 && (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_D)))
				{
					if (pGameInstance->Key_Up(DIK_U))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_FriendUseCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						m_tInfo.iFriendBar -= 500;
						m_fChangeDelay = 4.f;
						m_pSubChar->Set_Sub(false);
						m_pSubChar->Set_ChangeInfo(true);
						if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
							m_pSubChar->Set_Change(false, vPos);
						else
						{
							vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
							vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
							vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
							vPos += XMVector3Normalize(vLook) * 5.f;
							vPos += XMVector3Normalize(vRight) * 5.f;
							vPos.m128_f32[1] += 15.f;
							if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
								m_pSubChar->Set_Change(false, vPos);
							else
								m_pSubChar->Set_Change(false, vLastPos);
						}
						m_pSubChar->Set_BattleTarget(m_pBattleTarget);
						m_pSubChar->Set_SubSkill(2);
					}
				}
				else if (m_tInfo.iFriendBar >= 500 && pGameInstance->Key_Up(DIK_U))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_FriendUseCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					m_tInfo.iFriendBar -= 500;
					m_fChangeDelay = 4.f;
					m_pSubChar->Set_Sub(false);
					m_pSubChar->Set_ChangeInfo(true);
					if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
						m_pSubChar->Set_Change(false, vPos);
					else
					{
						vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
						vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
						vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
						vPos += XMVector3Normalize(vLook) * 5.f;
						vPos += XMVector3Normalize(vRight) * 5.f;
						vPos.m128_f32[1] += 15.f;
						if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
							m_pSubChar->Set_Change(false, vPos);
						else
							m_pSubChar->Set_Change(false, vLastPos);
					}
					m_pSubChar->Set_BattleTarget(m_pBattleTarget);
					m_pSubChar->Set_SubSkill(1);
				}
				else if (m_tInfo.iFriendBar >= 500 && pGameInstance->Key_Pressing(DIK_U))
				{
					m_fChangeTime += m_fDelta;
					if (m_fChangeTime > 0.5f)
					{
						m_tInfo.iFriendBar -= 500;
						m_tInfo.bSub = true;
						CUI_Manager::Get_Instance()->Set_1P(m_pSubChar);
						CUI_Manager::Get_Instance()->Set_1P_2(this);
						m_pSubChar->Set_ChangeDelay(1.f);
						m_pSubChar->Set_Sub(false);
						m_pSubChar->Change_Info(m_tInfo);
						m_pSubChar->Set_ChangeInfo(true);
						if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
							m_pSubChar->Set_Change(false, vPos);
						else
						{
							vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
							vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
							vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
							vPos += XMVector3Normalize(vLook) * 5.f;
							vPos += XMVector3Normalize(vRight) * 5.f;
							vPos.m128_f32[1] += 15.f;
							if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
								m_pSubChar->Set_Change(false, vPos);
							else
								m_pSubChar->Set_Change(false, vLastPos);
						}
						m_pSubChar->Set_SubSkill(0);
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
				if (m_tInfo.iFriendBar >= 500 && (pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN) || pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT)))
				{
					if (pGameInstance->Key_Up(DIK_V))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_FriendUseCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						m_tInfo.iFriendBar -= 500;
						m_fChangeDelay = 4.f;
						m_pSubChar->Set_Sub(false);
						m_pSubChar->Set_ChangeInfo(true);
						if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
							m_pSubChar->Set_Change(false, vPos);
						else
						{
							vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
							vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
							vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
							vPos += XMVector3Normalize(vLook) * 5.f;
							vPos += XMVector3Normalize(vRight) * 5.f;
							vPos.m128_f32[1] += 15.f;
							if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
								m_pSubChar->Set_Change(false, vPos);
							else
								m_pSubChar->Set_Change(false, vLastPos);
						}
						m_pSubChar->Set_BattleTarget(m_pBattleTarget);
						m_pSubChar->Set_SubSkill(2);
					}
				}
				else if (m_tInfo.iFriendBar >= 500 && pGameInstance->Key_Up(DIK_V))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_FriendUseCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					m_tInfo.iFriendBar -= 500;
					m_fChangeDelay = 4.f;
					m_pSubChar->Set_Sub(false);
					m_pSubChar->Set_ChangeInfo(true);
					if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
						m_pSubChar->Set_Change(false, vPos);
					else
					{
						vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
						vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
						vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
						vPos += XMVector3Normalize(vLook) * 5.f;
						vPos += XMVector3Normalize(vRight) * 5.f;
						vPos.m128_f32[1] += 15.f;
						if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
							m_pSubChar->Set_Change(false, vPos);
						else
							m_pSubChar->Set_Change(false, vLastPos);
					}
					m_pSubChar->Set_BattleTarget(m_pBattleTarget);
					m_pSubChar->Set_SubSkill(1);
				}
				else if (m_tInfo.iFriendBar >= 500 && pGameInstance->Key_Pressing(DIK_V))
				{
					m_fChangeTime += m_fDelta;
					if (m_fChangeTime > 0.5f)
					{
						m_tInfo.iFriendBar -= 500;
						m_tInfo.bSub = true;
						CUI_Manager::Get_Instance()->Set_2P(m_pSubChar);
						CUI_Manager::Get_Instance()->Set_2P_2(this);
						m_pSubChar->Set_ChangeDelay(1.f);
						m_pSubChar->Set_Sub(false);
						m_pSubChar->Change_Info(m_tInfo);
						m_pSubChar->Set_ChangeInfo(true);
						if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
							m_pSubChar->Set_Change(false, vPos);
						else
						{
							vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
							vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
							vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
							vPos += XMVector3Normalize(vLook) * 5.f;
							vPos += XMVector3Normalize(vRight) * 5.f;
							vPos.m128_f32[1] += 15.f;
							if (m_pSubChar->Get_NavigationCom()->Cheak_Cell(vPos))
								m_pSubChar->Set_Change(false, vPos);
							else
								m_pSubChar->Set_Change(false, vLastPos);
						}
						m_pSubChar->Set_SubSkill(0);
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
	}
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CTanjiro::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
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
	else if (g_iLevel == LEVEL_ADVRUI || g_iLevel == LEVEL_ADVAKAZA)
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


void CTanjiro::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if (m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_HIT)
		m_pModelCom->Reset_Anim(CTanjiro::ANIMID::ANIM_HIT);

	CTanjiroState* pState = new CHitState(_fPow, _bJumpHit);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Get_GuardHit(_int eType)
{
	CTanjiroState* pState;
	if (eType == CTanjiroState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_HIT_0);
		pState = new CGuardHitState(CTanjiroState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_HIT_1);
		pState = new CGuardHitState(CTanjiroState::STATE_TYPE::TYPE_LOOP);
	}
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Player_TakeDown(_float _fPow, _bool _bJumpHit)
{
	CTanjiroState* pState = new CTakeDownState(_fPow, _bJumpHit);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	CTanjiroState* pState = new CUpperHitState(eHitType, CTanjiroState::STATE_TYPE::TYPE_START, fBoundPower, fJumpPower, fKnockBackPower);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);

}

void CTanjiro::Play_Scene()
{
	CTanjiroState* pState = nullptr;

	switch (m_pBattleTarget->Get_PlayerType())
	{
	case Client::CCharacters::PLAYER_TANJIRO:
		break;
	case Client::CCharacters::PLAYER_KYOUJURO:
		break;
	case Client::CCharacters::PLAYER_RUI:
		pState = new HitCinema_Rui(HitCinema_Rui::SCENE_START);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
		break;
	case Client::CCharacters::PLAYER_AKAZA:
		break;
	case Client::CCharacters::PLAYER_NEZUKO:
		break;
	case Client::CCharacters::PLAYER_SHINOBU:
		break;
	case Client::CCharacters::PLAYER_END:
		break;
	default:
		break;
	}
}

void CTanjiro::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CTanjiroState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CTanjiroState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}




HRESULT CTanjiro::SetUp_ShaderResources()
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

HRESULT CTanjiro::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Tanjiro"), (CComponent**)&m_pModelCom)))
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
	else if(m_i1p == 20)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainNavi"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else if (g_iLevel == LEVEL_BATTLEENMU)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainBattle"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	
	return S_OK;
}

void CTanjiro::Set_Shadow()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_float4 vAt = vPos;

	vPos.x -= 20.f;
	vPos.y += 40.f;
	vPos.z -= 40.f;

	pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW, vPos, vAt);

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Check_QuestEvent(_float fTimeDelta)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if (!pUIManager->Get_SaveStory())
	{
		if (!m_bQuest1)
		{
			_vector vQuest1 = { -232.878f,37.521f,-338.528f,1.f };
			_float fDist1 = XMVectorGetX(XMVector3Length(vQuest1 - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist1 < 10.f)
			{
				m_bQuest1 = true;
				m_bStop = true;
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
				pUIManager->Set_Msg(TEXT("윽...냄새가 짙어졌어...이 앞에 무언가 있는듯해"));
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Dialog_00.wav"), fVOICE);
				pUIManager->Set_ClearCheck(true, 0);
			}
		}
		else if (!m_bQuest1MSG && m_bQuest1)
		{
			if (!pUIManager->Get_MsgOnOff())
			{
				m_bStop = false;
				m_bQuest1MSG = true;
			}
		}
		else if (!m_bQuest2)
		{
			_vector vQuest2 = { -287.899f,39.223f,-322.629f,1.f };
			_float fDist2 = XMVectorGetX(XMVector3Length(vQuest2 - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist2 < 10.f)
			{
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QusetCam();
				m_bQuest2 = true;
				m_bStop = true;
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("흐갸아아악~타스케테~"));
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Murata_Dialog_00.wav"), fVOICE);
			}
		}
		else if (!m_bQuest2MSG && m_bQuest1)
		{
			if (m_bQuest2_2MSG && !m_bQuest2_1MSG)
			{
				switch (pUIManager->Get_MsgCount())
				{
				case 0:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("귀살대원 '계'급 탄지로 입니다.지원 왔습니다."));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Dialog_01.wav"), fVOICE);
						m_bSoundCheck = true;
					}
					break;
				case 1:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
					pUIManager->Set_Msg(TEXT("앗! 지원군이 왔구나! 엥? 뭐라고? '계'급이라고?"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Murata_Dialog_01.wav"), fVOICE);
						m_bSoundCheck = false;
					}
					break;
				case 2:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
					pUIManager->Set_Msg(TEXT("왜 '주'가 아닌 거야?"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Murata_Dialog_02.wav"), fVOICE);
						m_bSoundCheck = true;
					}
					break;
				case 3:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
					pUIManager->Set_Msg(TEXT("계 같은 건 몇 명이 와도 똑같아!! 무의미해!!"));
					break;
				case 4:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("아빠 거미"));
					pUIManager->Set_Msg(TEXT("으캬아야아"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dialog_00.wav"), fVOICE);
						m_bSoundCheck = false;
					}
					break;
				case 5:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("아빠 거미"));
					pUIManager->Set_Msg(TEXT("내 가족에게 접근하지 마라!!"));
					pUIManager->Set_MainQuestOff();
					m_bQuest2_1MSG = true;
				//	dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_High(m_pRendererCom);
					break;
				default:
					break;
				}
			}
			if (m_bQuest2_2MSG && m_bQuest2_1MSG && !pUIManager->Get_MsgOnOff())
			{
				m_bStop = false;
				m_bQuest2MSG = true;

			}
			if (!m_bQuest2_2MSG && !pUIManager->Get_MsgOnOff())
			{
				pUIManager->Reset_MsgCount();
				m_bQuest2_2MSG = true;
			}
		}
	}
	else
	{
		if (!m_bQuest3)
		{
			_vector vQuest = { -834.618f,92.528f,-61.440f,1.f };
			_float fDist1 = XMVectorGetX(XMVector3Length(vQuest - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist1 < 10.f)
			{
				m_bQuest3 = true;
				m_bStop = true;
			}
		}
		else if (!m_bQuest3MSG && m_bQuest3)
		{
			if (m_bQuest3_2MSG && !m_bQuest3_1MSG)
			{
				switch (pUIManager->Get_MsgCount())
				{
				case 0:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("우리가족의 조용한 생활을 방해하지 마라"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Dialog_00.wav"), fVOICE);
						m_bSoundCheck = true;
					}
					break;
				case 1:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("행복한 꿈속에 있고 싶겠지. 이해해"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Dialog_06.wav"), fVOICE);
						m_bSoundCheck = false;
					}
					break;
				case 2:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("지금...뭐라고 했어?"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Dialog_01.wav"), fVOICE);
						m_bSoundCheck = true;
					}
					break;
				case 3:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("난 말이야, 자신의 역할을 이해 못하는 녀석은"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Dialog_02.wav"), fVOICE);
						m_bSoundCheck = false;
					}
					break;
				case 4:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("살아있을 필요가 없다고 생각해"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Dialog_03.wav"), fVOICE);
						m_bSoundCheck = true;
					}
					break;
				case 5:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("취소 안 해!! 내가 한 말은 틀리지 않았어!!\n이상한 건, 너다!!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Dialog_07.wav"), fVOICE);
						m_bSoundCheck = false;
					}
					break;
				case 6:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("성가시군.큰 소리 내지 말아줄래? 너하고는 맞지가 않네."));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Dialog_04.wav"), fVOICE);
						m_bSoundCheck = true;
					}
					break;
				case 7:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("넌 이제 됐어. 잘가라"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiSelect.wav"), fVOICE);
						m_bSoundCheck = false;
					}
					m_bQuest3_1MSG = true;
					break;
				default:
					break;
				}
			}
			if (m_bQuest3_2MSG && m_bQuest3_1MSG && !pUIManager->Get_MsgOnOff())
			{
				m_bStop = false;
				m_bQuest3MSG = true;

			}
			if (!m_bQuest3_2MSG && !pUIManager->Get_MsgOnOff())
			{
				pUIManager->Reset_MsgCount();
				m_bQuest3_2MSG = true;
			}
		}
	}
	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}


void CTanjiro::HandleInput(_float fTimeDelta)
{

	CTanjiroState* pNewState = m_pTanjiroState->HandleInput(this);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);
}

void CTanjiro::TickState(_float fTimeDelta)
{
	CTanjiroState* pNewState = m_pTanjiroState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);
}

void CTanjiro::LateTickState(_float fTimeDelta)
{
	CTanjiroState* pNewState = m_pTanjiroState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);

	if (m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_MOVE)
		m_fEffectTime += fTimeDelta;
	if (m_fEffectTime > 0.3f)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUN, this);

		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_Walk.wav"), fEFFECT);

		RELEASE_INSTANCE(CEffect_Manager);
		m_fEffectTime = 0.f;
	}
}
HRESULT CTanjiro::Ready_Parts()
{

	/* For.Weapon */
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("R_Hand_1_Lct");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroWeapon::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	if (m_i1p == 10)
		WeaponDesc.bStory = true;
	else
		WeaponDesc.bStory = false;
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroWeapon"), &WeaponDesc);
	if (nullptr == m_pWeapon)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTanjiro::Ready_Parts2()
{
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroSheath::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	if (m_i1p == 10)
		WeaponDesc.bStory = true;
	else
		WeaponDesc.bStory = false;
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSheath = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroSheath"), &WeaponDesc);
	if (nullptr == m_pSheath)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
void CTanjiro::Set_Info()
{
	m_tInfo.strName = TEXT("탄지로");
	m_tInfo.bOni = false;
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
CTanjiro * CTanjiro::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTanjiro*	pInstance = new CTanjiro(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTanjiro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTanjiro::Clone(void * pArg)
{
	CGameObject*	pInstance = new CTanjiro(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTanjiro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTanjiro::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pModelCom);
	Safe_Delete(m_pTanjiroState);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pSheath);
	Safe_Release(m_pNavigationCom);
}

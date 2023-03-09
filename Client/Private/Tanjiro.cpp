#include "stdafx.h"
#include "..\Public\Tanjiro.h"
#include "Layer.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "MGameHeart.h"
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
#include "Effect.h"
#include "HinoCami_CinemaState.h"
#include "TanjiroWeapon2.h"
// 오의히트
#include "TanjiroHitCinema_Rui.h"
#include "TanjiroHitCinema_Akaza.h"
#include "TanjiroHitCinema_Kyoujuro.h"
#include "TanjiroHitCinema_Nezuko.h"
#include "TanjiroHitCinema_Shinobu.h"
#include "TanjiroSplSkrStartState.h"
#include "Rui.h"
#include "Data_Manager.h"

#include "TanjiroTrain_CinemaState.h"
#include "Level_Loading.h"

#include "TanjiroAkazaScene.h"

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
	if (FAILED(Ready_Parts3()))
		return E_FAIL;

	m_WeaponWorld = *dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Get_CombinedWorld4x4();

	if (m_i1p != 10 && m_i1p != 20 && m_i1p != 33)
	{
		if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		{
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-0.435f, 16.413f, 212.616f, 1.f));
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
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);

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

	else if (m_i1p == 33)
	{
		Play_AkazaScene();

	}

	if (m_i1p != 33)
	{
		CTanjiroState* pState = new CIdleState();
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}
	Set_Info();
	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);


	m_ePlayerType = CCharacters::PLAYER_TYPE::PLAYER_TANJIRO;


	return S_OK;
}

void CTanjiro::Tick(_float fTimeDelta)
{

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	if (m_i1p == 33)
	{
		m_bDeadTime += fTimeDelta;
		if (m_bDeadTime >= 17.f)
		{
			Set_Dead();
			return;
		}
		HandleInput(fTimeDelta);
		TickState(fTimeDelta);
		CHierarchyNode*		pSocket = nullptr;
		_matrix			matColl;
		if (g_iLevel == LEVEL_ADVRUI || g_iLevel == LEVEL_ADVAKAZA)
		{
			pSocket = m_pModelADVCom->Get_BonePtr("C_Spine_3");
			if (nullptr == pSocket)
				return;
			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelADVCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());
		}
		else
		{
			pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");


			if (nullptr == pSocket)
				return;
			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());
		}
		m_pSphereCom->Update(matColl);

		if (g_iLevel == LEVEL_ADVRUI)
		{
			Set_Shadow();
			Check_QuestEvent(fTimeDelta);
		}
		else if (g_iLevel == LEVEL_ADVAKAZA ||
			g_iLevel == LEVEL_BATTLEENMU ||
			g_iLevel == LEVEL_BOSSENMU)
		{
			Set_Shadow();
			Check_QuestTrainEvent(fTimeDelta);
		}
	}


	if (m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_JUMP
		|| m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_CHANGE ||
		m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_JUMP_ATTACK || m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_SKILL_KAGURA_COMMON)
	{
		m_tInfo.bJump = true;
	}


	if (m_i1p != 33)
	{

		_float4 vPos;
		XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

		if (!m_bChange)
		{
			if (m_bStorySpl)
				StorySpl(fTimeDelta);
			__super::Tick(fTimeDelta);
			m_fEffectStartTime = 0.f;

			if (g_iLevel == LEVEL_ADVRUI)
			{
				if (m_iStoneHitTime > 0.f)
				{
					m_bHitRender = true;
					if (m_fHitRenderTime > 0.1f)
					{
						m_fHitRenderTime = 0.f;
						m_bHitRender = !m_bHitRender;
					}
					m_fHitRenderTime += fTimeDelta;
					m_iStoneHitTime -= fTimeDelta;
				}
				if (m_fSphereHitTime > 0.f)
				{
					m_bHitRender = true;
					if (m_fHitRenderTime > 0.1f)
					{
						m_fHitRenderTime = 0.f;
						m_bHitRender = !m_bHitRender;
					}
					m_fHitRenderTime += fTimeDelta;
					m_iStoneHitTime -= fTimeDelta;
				}
			}
			if (m_bBattleStart && m_i1p != 33)
			{
				if (g_iLevel != LEVEL_BOSSENMU)
				{
					CTanjiroState* pState = new CBattleStartState();
					m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
				}
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

			CHierarchyNode*		pSocket = nullptr;
			_matrix			matColl;
			if (g_iLevel == LEVEL_ADVRUI || g_iLevel == LEVEL_ADVAKAZA)
			{
				pSocket = m_pModelADVCom->Get_BonePtr("C_Spine_3");
				if (nullptr == pSocket)
					return;
				matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelADVCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());
			}
			else
			{
				pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");

				if (nullptr == pSocket)
					return;
				matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());
			}

			m_pSphereCom->Update(matColl);

			if (g_iLevel == LEVEL_ADVRUI)
			{
				Set_Shadow();
				Check_QuestEvent(fTimeDelta);
			}
			else if (g_iLevel == LEVEL_ADVAKAZA ||
				g_iLevel == LEVEL_BATTLEENMU ||
				g_iLevel == LEVEL_BOSSENMU)
			{
				Set_Shadow();
				Check_QuestTrainEvent(fTimeDelta);
			}
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
}

void CTanjiro::Late_Tick(_float fTimeDelta)
{


	if (!m_bChange || m_i1p == 33)
	{
		LateTickState(fTimeDelta);
		if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		{
			m_pWeapon->Tick(fTimeDelta);
			m_pSheath->Tick(fTimeDelta);
		}
		else
		{
			m_pWeapon2->Tick(fTimeDelta);
		}
		if (m_bSplSkl)
		{
			Check_Spl();
		}


		if (!m_bRender && m_bSceneRender)
		{
			if (g_iLevel == LEVEL_ADVRUI)
			{
				if (m_iStoneHitTime > 0.f)
				{
					if (!m_bHitRender)
					{
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
						dynamic_cast<CTanjiroWeapon2*>(m_pWeapon2)->Set_Render(true);
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon2);
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon2);
					}
				}
				else
				{
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
					dynamic_cast<CTanjiroWeapon2*>(m_pWeapon2)->Set_Render(true);
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon2);
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon2);
				}

				if (m_fSphereHitTime > 0.f)
				{
					if (!m_bHitRender)
					{
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
						dynamic_cast<CTanjiroWeapon2*>(m_pWeapon2)->Set_Render(true);
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon2);
						m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon2);
					}
				}
				else
				{
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
					dynamic_cast<CTanjiroWeapon2*>(m_pWeapon2)->Set_Render(true);
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon2);
					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon2);
				}
			}
			else
			{
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			}
			if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
			{
				dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Set_Render(true);
				dynamic_cast<CTanjiroSheath*>(m_pSheath)->Set_Render(true);

				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pSheath);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pSheath);
			}
			else if (g_iLevel == LEVEL_ADVAKAZA)
			{
				dynamic_cast<CTanjiroWeapon2*>(m_pWeapon2)->Set_Render(true);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon2);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon2);
			}
	
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

	_uint		iNumMeshes = 0;
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

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

		CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

		if (g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU && pUI_Manager->Get_Sel2P() != 8 && m_i1p != 33)
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
		RELEASE_INSTANCE(CUI_Manager);
	}
	else
	{
		iNumMeshes = m_pModelADVCom->Get_NumMeshContainers();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelADVCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (i == 0 || i == 1)
			{
				if (FAILED(m_pModelADVCom->SetUp_Material(m_pShaderCom, "g_MaskTexture", i, aiTextureType_NORMALS)))
					return E_FAIL;

				if (FAILED(m_pModelADVCom->Render(m_pShaderCom, i, 2)))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pModelADVCom->Render(m_pShaderCom, i, 0)))
					return E_FAIL;
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
	else
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float))))
		return E_FAIL;

	_uint		iNumMeshes = 0;
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
				return E_FAIL;

		}
	}
	else
	{
		iNumMeshes = m_pModelADVCom->Get_NumMeshContainers();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelADVCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;

			if (FAILED(m_pModelADVCom->Render(m_pShaderCom, i, 1)))
				return E_FAIL;

		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


void CTanjiro::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	//STATE_SKILL_WATERMILL,
	//STATE_SKILL_WINDMILL,
	//STATE_SKILL_COMMON,
	//STATE_SKILL_KAGURA_COMMON,
	//STATE_SKILL_KAGURA_MOVE,
	//STATE_SKILL_KAGURA_SPHERE,



	if (m_tInfo.iAccComboDmg >= 300)
	{
		Reset_AccComboDmg();
		Set_GodMode(true);
		CTanjiroState* pState = new CUpperHitState(CCharacters::HIT_KNOCKBACK, CTanjiroState::STATE_TYPE::TYPE_START, 20.f, 30.f, 7.f);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}


	else if (m_pTanjiroState->Get_TanjiroState() != CTanjiroState::STATE_SKILL_KAGURA_SPHERE)
	{

		CTanjiroState* pState = new CHitState(_fPow, _bJumpHit);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}

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
	if (m_tInfo.iAccComboDmg >= 300)
	{
		Reset_AccComboDmg();
		Set_GodMode(true);
		CTanjiroState* pState = new CUpperHitState(CCharacters::HIT_KNOCKBACK, CTanjiroState::STATE_TYPE::TYPE_START, 20.f, 30.f, 7.f);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}


	else if (m_pTanjiroState->Get_TanjiroState() != CTanjiroState::STATE_SKILL_KAGURA_SPHERE)
	{
		CTanjiroState* pState = new CTakeDownState(_fPow, _bJumpHit);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}
}

void CTanjiro::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	if (m_tInfo.iAccComboDmg >= 300)
	{
		Reset_AccComboDmg();
		Set_GodMode(true);
		CTanjiroState* pState = new CUpperHitState(CCharacters::HIT_KNOCKBACK, CTanjiroState::STATE_TYPE::TYPE_START, 20.f, 30.f, 7.f);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}

	else if (m_pTanjiroState->Get_TanjiroState() != CTanjiroState::STATE_SKILL_KAGURA_SPHERE)
	{
		CTanjiroState* pState = new CUpperHitState(eHitType, CTanjiroState::STATE_TYPE::TYPE_START, 15.f, 20.f, fKnockBackPower);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	}

}

void CTanjiro::Play_Scene()
{
	CTanjiroState* pState = nullptr;
	CGameInstance* pGameInstance = nullptr;

	switch (m_pBattleTarget->Get_PlayerType())
	{
	case Client::CCharacters::PLAYER_TANJIRO:
		break;
	case Client::CCharacters::PLAYER_KYOUJURO:
		pState = new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_START);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
		break;
	case Client::CCharacters::PLAYER_RUI:
		pState = new CHitCinema_Rui(CHitCinema_Rui::SCENE_START);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
		break;
	case Client::CCharacters::PLAYER_AKAZA:
		pState = new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_START);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
		break;
	case Client::CCharacters::PLAYER_NEZUKO:
		pState = new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_START);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
		break;
	case Client::CCharacters::PLAYER_SHINOBU:
		pState = new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_START);
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
		break;
	case Client::CCharacters::PLAYER_END:
		break;
	default:
		break;
	}
}

void CTanjiro::Play_AkazaScene()
{
	m_bAkazaScene = true;

	CTanjiroState* pState = new CTanjiroAkazaScene();
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Set_JumpState(_float fJumpHeight, _float fJumpTime, _float fJumpTimer)
{
	CTanjiroState* pState = new CTrain_CinemaState(CTrain_CinemaState::SCENE_START, fJumpHeight, fJumpTime, fJumpTimer, 0.f);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CTanjiroState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CTanjiroState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Set_StoryPowerUp()
{
	m_bIsKagura = true;
	m_tInfo.iPowerIndex = 1;
	m_tInfo.fPowerUp = 1.5f;
	m_tInfo.fPowerUpTime = 9999.f;
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, this);
	pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_TAN, this);
	RELEASE_INSTANCE(CEffect_Manager);
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float))))
		return E_FAIL;

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
	if (g_iLevel == LEVEL_ADVRUI)
		TransformDesc.fSpeedPerSec = 20.f;
	else
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
	if (FAILED(__super::Add_Components(TEXT("Com_ModelADV"), LEVEL_STATIC, TEXT("TanjiroADV"), (CComponent**)&m_pModelADVCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	if (g_iLevel == LEVEL_ADVRUI || g_iLevel == LEVEL_ADVAKAZA)
		ColliderDesc.vScale = _float3(80.f, 80.f, 80.f);
	else
		ColliderDesc.vScale = _float3(130.f, 130.f, 130.f);

	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;
	if (m_i1p == 10)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else if (m_i1p == 20)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainNavi"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else if (g_iLevel == LEVEL_BATTLEENMU)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TrainBattle"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
	}
	else if (g_iLevel == LEVEL_BOSSENMU)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Enmu_Navi"), (CComponent**)&m_pNavigationCom)))
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

	_float fValue = 1.f;
	if (LEVEL_BATTLEENMU == g_iLevel)
		fValue = 4.8f;

	vPos.x -= 20.f * fValue;
	vPos.y += 40.f * fValue;
	vPos.z -= 40.f * fValue;

	pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW, vPos, vAt);

	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Check_QuestEvent(_float fTimeDelta)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	if (!m_bItemBoxMsg)
	{
		_vector vItemBoxClearPos = { -235.040131f, 42.7410583f, -197.561172f, 1.f };
		_float fDist = XMVectorGetX(XMVector3Length(vItemBoxClearPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

		if (pUIManager->Get_ItemBoxCheck() && fDist < 10.f)
		{
			switch (pUIManager->Get_MsgCount())
			{
			case 0:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
				pUIManager->Set_Msg(TEXT("휴... 지독한 혈귀놈들.."));
				break;
			default:
				pUIManager->Set_MsgOff();
				pUIManager->Reset_MsgCount();
				for (_uint i = 0; i < 3; ++i)
				{
					dynamic_cast<CMGameHeart*>(pUIManager->Get_HeartUI(i))->Set_RenderOn(false);
				}
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_TopView(false);
				m_iHeart = 2;
				m_bItemBoxMsg = true;
				break;
			}
		}
	}

	if (!pUIManager->Get_SaveStory())
	{
		if (!m_bQuest1)
		{
			_vector vQuest1 = { -232.878f,37.521f,-338.528f,1.f };
			_float fDist1 = XMVectorGetX(XMVector3Length(vQuest1 - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist1 < 10.f)
			{
				switch (pUIManager->Get_MsgCount())
				{
				case 0:
					m_bStop = true;
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("윽...냄새가 짙어졌어...이 앞에 무언가 있는듯해"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_00.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				default:
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					pUIManager->Set_MsgOff();
					pUIManager->Reset_MsgCount();
					pUIManager->Set_ClearCheck(true, 0);
					m_bSoundCheck = false;
					m_bQuest1 = true;
					m_bStop = false;
					break;
				}
			}
		}
		else if (!m_bQuest2)
		{
			_vector vQuest2 = { -287.899f,39.223f,-322.629f,1.f };
			_float fDist2 = XMVectorGetX(XMVector3Length(vQuest2 - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist2 < 10.f)
			{
				vQuest2 = { -280.015f,38.593f,-328.086f,1.f };
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vQuest2);
				m_pNavigationCom->Find_CurrentCellIndex(vQuest2);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QusetCam();
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_RUIDAD_START);
				m_bQuest2 = true;
				m_bStop = true;
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("흐갸아아악~타스케테~"));
				CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_00.wav"), g_fDialog);
				//pUIManager->Set_MsgOff();
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
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_01.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 1:
					pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
					pUIManager->Set_Msg(TEXT("앗! 지원군이 왔구나! 엥? 뭐라고? '계'급이라고?"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_01.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 2:
					pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
					pUIManager->Set_Msg(TEXT("왜 '주'가 아닌 거야?"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_02.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 3:
					pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
					pUIManager->Set_Msg(TEXT("계 같은 건 몇 명이 와도 똑같아!! 무의미해!!"));
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 4:
					pUIManager->Set_MsgName(TEXT("아빠 거미"));
					pUIManager->Set_Msg(TEXT("내 가족에게 접근하지 마라!!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("RuiDad_Dialog_00.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					//	dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_High(m_pRendererCom);
					break;
				default:
					pUIManager->Set_MainQuestOff();
					pUIManager->Reset_MsgCount();
					m_bQuest2_1MSG = true;
					pUIManager->Set_MsgOff();
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
			else if (!m_bQuest2_2MSG && pUIManager->Get_MsgOnOff())
			{
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgOff();
			}
		}
	}
	else
	{
		if (!m_bQuestStoneBallMSG)
		{
			_vector vStoneMGame = { -556.3474f, 53.5169f, -61.9126f, 1.f };
			_float fDistStoneMGame = XMVectorGetX(XMVector3Length(vStoneMGame - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));
			if (fDistStoneMGame < 10.f)
			{
				switch (pUIManager->Get_MsgCount())
				{
				case 0:
					m_bStop = true;
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("큰 바위들이 굴러오고 있어..."));
					break;
				case 1:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("위에서 혈귀 냄새가 나는 것 같아"));
					break;
				case 2:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("조심히 올라가 보자"));
					break;
				default:
					pUIManager->Set_MsgOff();
					pUIManager->Reset_MsgCount();
					m_bQuestStoneBallMSG = true;
					for (_uint i = 0; i < 3; ++i)
					{
						dynamic_cast<CMGameHeart*>(pUIManager->Get_HeartUI(i))->Set_RenderOn(true);
					}
					m_bStop = false;
					break;
				}
			}
		}
		else if (m_bQuestStoneBallMSG && !m_bStoneEnd)
		{
			_vector vStoneMGame = { -815.138184f, 92.4922714f, -62.1797104f, 1.f };
			_float fDistStoneMGame = XMVectorGetX(XMVector3Length(vStoneMGame - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));
			if (fDistStoneMGame < 7.f)
			{
				switch (pUIManager->Get_MsgCount())
				{
				case 0:
					m_bStop = true;
					for (_uint i = 0; i < 3; ++i)
					{
						dynamic_cast<CMGameHeart*>(pUIManager->Get_HeartUI(i))->Set_RenderOn(false);
					}
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("휴... 응...?"));
					break;
				case 1:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("앞에서 강한 혈귀의 냄새가 난다..."));
					break;
				default:
					pUIManager->Set_MsgOff();
					pUIManager->Reset_MsgCount();
					m_bStoneEnd = true;
					m_bStop = false;
					break;
				}
			}
		}
		if (!m_bQuest3)
		{
			_vector vQuest = { -834.618f,92.528f,-61.440f,1.f };
			_float fDist1 = XMVectorGetX(XMVector3Length(vQuest - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist1 < 10.f)
			{
				_vector vPos = { -850.479f, 93.596f,-61.984f,1.f };
				m_pNavigationCom->Find_CurrentCellIndex(vPos);
				Set_NavigationHeight(vPos);
				vPos.m128_f32[1] = m_pNavigationCom->Get_NavigationHeight().y;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
				m_pTransformCom->LookAt(m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QusetCam();
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_RUI_START);
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
					pUIManager->Set_Msg(TEXT("뭘 보고 있어? 구경거리가 아니야"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->BGM_Stop();
						CSoundMgr::Get_Instance()->PlayBGM(TEXT("Rui_Meeting.wav"), g_fBGM);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Rui_Dialog_00.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 1:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("...무슨 짓이야? 동료 사이 아니였어?"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_06.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 2:
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("동료? 그런 얄팍한 것과 같은 취급하지 마"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Rui_Dialog_01.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 3:
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("우린 가족이다. 강한 인연으로 묶여 있어"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Rui_Dialog_02.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 4:
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("그리고 이건 나와 누나의 문제야"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Rui_Dialog_03.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 5:
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("쓸데없이 참견한다면 토막 내 주겠어"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Rui_Dialog_04.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 6:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("가족도 동료도 강한 인연으로 묶여 있으면 어느쪽이든 똑같이 소중해"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_07.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 7:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("그리고 강한 인연으로 엮인 자에겐 신뢰의 냄새가 나!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_08.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 8:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("하지만 너희들에게선 공포와 증오와 혐오의 냄새만 나!"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_09.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 9:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("이런 건 인연이라고 안 해! 모조품...."));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_10.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 10:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("가짜다!"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_11.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 11:
					pUIManager->Set_MsgName(TEXT("루이"));
					pUIManager->Set_Msg(TEXT("너...지금... 뭐라고 했어?"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->BGM_Stop();
						CSoundMgr::Get_Instance()->PlayBGM(TEXT("Adv_Menu.wav"), g_fBGM);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Rui_Dialog_05.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 12:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("몇 번이고 말해주마!! 너의 인연은... 가짜다!!"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_12.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				default:
					pUIManager->Set_MsgOff();
					pUIManager->Reset_MsgCount();
					m_bQuest3_1MSG = true;
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

void CTanjiro::Check_QuestTrainEvent(_float fTimeDelta)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if (!pUIManager->Get_SaveStory())
	{
		if (!m_bStoryStartCheck)
		{
			if (!m_bStory)
			{
				_vector vQuestStart = { 0.414626598f, 5.70602703f, 382.156799f,1.f };
				_float fDistStart = XMVectorGetX(XMVector3Length(vQuestStart - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

				if (fDistStart < 3.f)
				{
					m_bStory = true;
					pUIManager->Reset_MsgCount();
					m_bStop = true;
				}
			}
			else if (m_bStory)
			{
				switch (pUIManager->Get_MsgCount())
				{
				case 0:
					pUIManager->Set_MsgOn();
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("사람들이 잠들어 있다..."));
					break;
				case 1:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("혈귀가 수를 쓴 거야"));
					break;
				default:
					pUIManager->Reset_MsgCount();
					pUIManager->Set_MsgOff();
					m_bStop = false;
					m_bStoryStartCheck = true;
					break;
				}
			}
		}


		if (!m_bQuest1 && m_bStory)
		{
			_vector vQuest1 = { 4.862f, 5.747f,283.194f,1.f };
			_float fDist1 = XMVectorGetX(XMVector3Length(vQuest1 - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

			if (fDist1 < 3.f)
			{
				m_bQuest1 = true;
				m_bStop = true;
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
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("카마도 소년!"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_02.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 1:
					pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
					pUIManager->Set_Msg(TEXT("렌고쿠 씨!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_03.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 2:
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("여기 오는 길에 꽤나 참격을 날리며 왔으니"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_04.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 3:
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("혈귀도 재생하는 데 시간이 걸리겠지만 여유는 없다!!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_05.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 4:
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("간단히 말하지! 이 기차는 8량 편성이다!"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_06.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 5:
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("나는 후방 5량을 지키도록 하지!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_07.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 6:
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("나머지 3량은 너와 네 여동생이 지킨다!"));
					if (!m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_08.wav"), g_fDialog);
						m_bSoundCheck = true;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				case 7:
					pUIManager->Set_MsgName(TEXT("렌고쿠 쿄주로"));
					pUIManager->Set_Msg(TEXT("나도 혈귀를 찾으며 싸우겠다!\n너도 마음을 다잡아라!"));
					if (m_bSoundCheck)
					{
						CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
						CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_09.wav"), g_fDialog);
						m_bSoundCheck = false;
					}
					CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
					if (!m_bIsPlaying)
						pUIManager->Set_MsgCount(1);
					break;
				default:
					pUIManager->Set_MsgOff();
					m_bQuest2_1MSG = true;
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
		else
		{
			if (!m_bQuest3)
			{
				_vector vQuest1 = { -0.175f, 5.707f,214.807f,1.f };
				_float fDist1 = XMVectorGetX(XMVector3Length(vQuest1 - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

				if (fDist1 < 3.f)
				{
					m_bQuest3 = true;
					m_bStop = true;
					CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_ADV_ENMU);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QusetCam();
					RELEASE_INSTANCE(CGameInstance);
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
						pUIManager->Set_MsgName(TEXT("엔무"));
						pUIManager->Set_Msg(TEXT("꿈꾸면서 죽을 수 있다는 건 참 행복한 일이지"));
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_10.wav"), g_fDialog);
							m_bSoundCheck = true;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					case 1:
						pUIManager->Set_MsgName(TEXT("엔무"));
						pUIManager->Set_Msg(TEXT("아무리 강한 혈귀 사냥꾼이라도 마찬가지야"));
						if (m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_11.wav"), g_fDialog);
							m_bSoundCheck = false;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					case 2:
						pUIManager->Set_MsgName(TEXT("엔무"));
						pUIManager->Set_Msg(TEXT("인간의 원동력은 마음이고 정신이지"));
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_12.wav"), g_fDialog);
							m_bSoundCheck = true;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					case 3:
						pUIManager->Set_MsgName(TEXT("엔무"));
						pUIManager->Set_Msg(TEXT("'정신의 핵'을 파괴하면 되는 거야\n그럼 살아있는 시체가 돼서 죽이는 것도 간단하지"));
						if (m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_13.wav"), g_fDialog);
							m_bSoundCheck = false;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					case 4:
						pUIManager->Set_MsgName(TEXT("엔무"));
						pUIManager->Set_Msg(TEXT("인간의 마음 따위는 모두 똑같아\n유리 세공품처럼 무르고 약하거든"));
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_14.wav"), g_fDialog);
							m_bSoundCheck = true;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					case 5:
						pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
						pUIManager->Set_Msg(TEXT("인간의 마음속을 더러운 발로 짓밟지 마!!"));
						if (m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_15.wav"), g_fDialog);
							m_bSoundCheck = false;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					case 6:
						pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
						pUIManager->Set_Msg(TEXT("난 널 용서 못 해!!"));
						if (!m_bSoundCheck)
						{
							CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
							CSoundMgr::Get_Instance()->PlayDialog(TEXT("MugenTrain_Dialog_16.wav"), g_fDialog);
							m_bSoundCheck = true;
						}
						CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
						if (!m_bIsPlaying)
							pUIManager->Set_MsgCount(1);
						break;
					default:
						pUIManager->Reset_MsgCount();
						pUIManager->Set_MsgOff();
						m_bQuest3_1MSG = true;
						break;
					}
				}
				if (m_bQuest3_2MSG && m_bQuest3_1MSG && !pUIManager->Get_MsgOnOff())
				{
					m_bStop = false;
					m_bQuest3MSG = true;
					pUIManager->Set_EnmuBattle(true);
				}
				if (!m_bQuest3_2MSG && !pUIManager->Get_MsgOnOff())
				{
					pUIManager->Reset_MsgCount();
					m_bQuest3_2MSG = true;
				}
			}
		}
	}
	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CTanjiro::Check_StartTrainEvent(_float fTimeDelta)
{
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

		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_Walk.wav"), g_fEffect);

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

HRESULT CTanjiro::Ready_Parts3()
{
	CHierarchyNode*		pSocket = m_pModelADVCom->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroWeapon2::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelADVCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	if (m_i1p == 10)
		WeaponDesc.bStory = true;
	else
		WeaponDesc.bStory = false;
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon2 = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroWeapon2"), &WeaponDesc);
	if (nullptr == m_pWeapon2)
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
	m_iHeart = 2;
}
void CTanjiro::Check_Spl()
{
	if (!m_bSplEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO1_SLASH1, &m_WeaponWorld);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO1_SLASH2, &m_WeaponWorld);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_PROJ1, this);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO1_SWORD, &m_WeaponWorld);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bSplEffect = true;
	}

	m_WeaponWorld = *dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Get_CombinedWorld4x4();
	//m_WeaponWorld = dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Get_CombinedWorld4x4();
	//XMStoreFloat4x4( &m_WeaponWorld, (dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Get_CombinedWorldMatrix()));
}
void CTanjiro::StorySpl(_float fTimeDelta)
{

	dynamic_cast<CRui*>(m_pBattleTarget)->Set_StoryDead();
	m_pBattleTarget->Play_Scene();
	CTanjiroState* pState = new CHinoCami_CinemaState(CHinoCami_CinemaState::SCENE_START);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
	m_bStorySpl = false;
	m_bStorySplEnd = true;

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
	Safe_Release(m_pModelADVCom);
	Safe_Delete(m_pTanjiroState);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pWeapon2);
	Safe_Release(m_pSheath);
	Safe_Release(m_pNavigationCom);
}

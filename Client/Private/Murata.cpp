#include "stdafx.h"
#include "..\Public\Murata.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"
#include "Tanjiro.h"
#include "Level_Loading.h"
#include "SoundMgr.h"

CMurata::CMurata(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CMurata::CMurata(const CMurata & rhs)
	: CCharacters(rhs)
{
}

HRESULT CMurata::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMurata::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	m_pTransformCom->Set_Scale(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	_vector vPos;
	if (CUI_Manager::Get_Instance()->Get_SaveStory())
	{
		vPos = XMLoadFloat4(&CUI_Manager::Get_Instance()->Get_TargetPos());
		m_bClearQuest = true;
		m_pModelCom->Set_CurrentAnimIndex(2);
	}
	else
	{
		vPos = { -323.555f,42.254f,-321.173f,1.f };
		m_pModelCom->Set_CurrentAnimIndex(0);
	}
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pNavigationCom->Find_CurrentCellIndex(vPos);
	m_pBattleTarget = tCharacterDesc.pSubChar;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QuestNPC(this);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CMurata::Tick(_float fTimeDelta)
{
	if (!m_bClearQuest && dynamic_cast<CTanjiro*>(m_pBattleTarget)->Get_Quest2())
	{
		if (!m_bQuestStart)
		{
			m_bQuestStart = true;
			m_pTransformCom->Set_PlayerLookAt(m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		}
		_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));
		
		if (fDist <= 3.f)
		{
			CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
			pUIManager->Set_NpcForUI(this);
			m_bInteractionCheck = true;
			
			if (!m_bSetPos)
			{
				_float4 vPos;
				XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
				pUIManager->Set_TargetPos(vPos);
				m_bSetPos = true;
			}
				m_bQuestStop = true;
				dynamic_cast<CTanjiro*>(m_pBattleTarget)->Set_Stop(false);
				m_pModelCom->Set_CurrentAnimIndex(3);

				_float m_fCurrentDuration = m_pModelCom->Get_CurrentTime_Index(3);

				if (m_fCurrentDuration > 115.5f)
				{
					m_pModelCom->Reset_Anim(3);
					m_pModelCom->Set_CurrentTime_Index(3, 35.f);
				}
			
			RELEASE_INSTANCE(CUI_Manager);
	
		}
		else if(!m_bQuestStop)
		{
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}
	}
	else
	{
		//퀘스트 클리어후
		if (!m_bQuestStop2)
		{
			m_pTransformCom->LookAt(m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			m_bQuestStop2 = true;
		}
		Check_Event();
	}
}

void CMurata::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));


	if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
	{
		if (!m_bClearQuest && dynamic_cast<CTanjiro*>(m_pBattleTarget)->Get_Quest2())
		{
			if (fDist < 70.f)
			{
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
				m_pModelCom->Play_Animation(fTimeDelta);
			}
		}
		else
		{
			if (fDist < 40.f)
			{
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
				m_pModelCom->Play_Animation(fTimeDelta);
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CMurata::Render()
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
		if (i == 0)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
				return E_FAIL;
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CMurata::Render_ShadowDepth()
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



HRESULT CMurata::SetUp_ShaderResources()
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

HRESULT CMurata::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Murata"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Enmu_Normal"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CMurata::Check_Event()
{
	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if (fDist < 5.f)
	{
		if (!m_bMsgEnd)
		{
			if (!m_MsgReset)
			{
				pUIManager->Set_InteractionOn();
				pUIManager->Set_NpcForUI(this);
				m_bInteractionCheck = true;
			}
		}
		else
		{
			pUIManager->Set_InteractionOff();
			pUIManager->Set_NpcForUI(nullptr);
			m_bInteractionCheck = false;
		}

		if (!m_bMsgStart && !m_bMsgEnd && pGameInstance->Key_Down(DIK_F))
		{
			pUIManager->Set_InteractionOff();
			m_bMsgStart = true;
			pUIManager->Set_NpcForUI(nullptr);
			m_bInteractionCheck = false;
			if (!m_MsgReset)
			{
				pUIManager->Reset_MsgCount();
				m_MsgReset = true;
			}
		}
		if (!m_bMsgEnd && m_bMsgStart)
		{
			switch (pUIManager->Get_MsgCount())
			{
			case 0:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
				pUIManager->Set_Msg(TEXT("무슨일이 있었던거에요?"));
				if (!m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_02.wav"), g_fDialog);
					m_bSoundCheck = true;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 1:
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("까, 까마귀한테 지령을 전달받고....!"));
				if (m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_03.wav"), g_fDialog);
					m_bSoundCheck = false;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 2:
				pUIManager->Set_Msg(TEXT("대원 열 명이 이곳에 왔어"));
				if (!m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_04.wav"), g_fDialog);
					m_bSoundCheck = true;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 3:
				pUIManager->Set_Msg(TEXT("산에 들어온 지 얼마 지나지 않았는데... 대원들... 대원들끼리...."));
				if (m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_05.wav"), g_fDialog);
					m_bSoundCheck = false;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 4:
				pUIManager->Set_Msg(TEXT("칼부림이 시작돼서...!!"));
				if (!m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Murata_Dialog_06.wav"), g_fDialog);
					m_bSoundCheck = true;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 5:
				pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
				pUIManager->Set_Msg(TEXT("진정하세요. 남은 대원들은 제가 빨리 찾아볼게요"));
				if (m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Tanjiro_Dialog_03.wav"), g_fDialog);
					m_bSoundCheck = false;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			default:
				pUIManager->Set_MsgOff();
				pUIManager->Set_QuestCount(1);
				pUIManager->Set_MainQuestOn();
				m_bMsgEnd = true;
				break;
			}
		}
		
	}
	else
	{
		if (!m_bMsgEnd)
			pUIManager->Set_InteractionOff();

		pUIManager->Set_NpcForUI(nullptr);
		m_bInteractionCheck = false;
	}
	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CMurata::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CMurata::Get_GuardHit(_int eType)
{
}

void CMurata::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CMurata::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void CMurata::Play_Scene()
{
}

CMurata * CMurata::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMurata*	pInstance = new CMurata(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMurata"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMurata::Clone(void * pArg)
{
	CGameObject*	pInstance = new CMurata(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMurata"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMurata::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
}

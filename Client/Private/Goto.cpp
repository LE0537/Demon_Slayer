#include "stdafx.h"
#include "..\Public\Goto.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"
#include "SoundMgr.h"

CGoto::CGoto(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CGoto::CGoto(const CGoto & rhs)
	: CCharacters(rhs)
{
}

HRESULT CGoto::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGoto::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;
	
	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	m_pTransformCom->Set_Scale(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	m_pBattleTarget = tCharacterDesc.pSubChar;
	m_pModelCom->Set_CurrentAnimIndex(0);
	return S_OK;
}

void CGoto::Tick(_float fTimeDelta)
{
	
}

void CGoto::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));


	if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
	{
		if (fDist < 30.f)
		{
			Check_Event(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			m_pModelCom->Play_Animation(fTimeDelta);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CGoto::Render()
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

HRESULT CGoto::Render_ShadowDepth()
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
void CGoto::Check_Event(_float fTimeDelta)
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
				pUIManager->Set_MsgName(TEXT("±Í»ì´ë¿ø °íÅä"));
				pUIManager->Set_Msg(TEXT("À¸À¸...À¸À¹...µµ,µµ¿ÍÁà...."));
				if (!m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Goto_Dialog_00.wav"), g_fDialog);
					m_bSoundCheck = true;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 1:
				pUIManager->Set_MsgName(TEXT("Ä«¸¶µµ ÅºÁö·Î"));
				pUIManager->Set_Msg(TEXT("±¦Âú¾Æ?"));
				if (m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->Effect_Stop(SOUND_DIALOG);
					CSoundMgr::Get_Instance()->PlayDialog(TEXT("Goto_Dialog_05.wav"), g_fDialog);
					m_bSoundCheck = false;
				}
				CSoundMgr::Get_Instance()->Dialog_End(&m_bIsPlaying);
				if (!m_bIsPlaying)
					pUIManager->Set_MsgCount(1);
				break;
			case 2:
				pUIManager->Set_MsgName(TEXT("±Í»ì´ë¿ø °íÅä"));
				pUIManager->Set_Msg(TEXT("¾öÃ» °­ÇÑ Ç÷±ÍÇÑÅ× ´çÇß¾î..."));
				m_fMsgTime += fTimeDelta;
				if (m_fMsgTime >= 2.f)
				{
					pUIManager->Set_MsgCount(1);
					m_fMsgTime = 0.f;
				}
				break;
			case 3:
				pUIManager->Set_Msg(TEXT("±× Ç÷±Í°¡ ³Ê¹« ½Ú ³ª¸ÓÁö ³» ÀÏ·ûµµµµ ºÎ·¯Á³¾î..."));
				m_fMsgTime += fTimeDelta;
				if (m_fMsgTime >= 2.f)
				{
					pUIManager->Set_MsgCount(1);
					m_fMsgTime = 0.f;
				}
				break;
			case 4:
				pUIManager->Set_Msg(TEXT("±× ³à¼®À» Á¶½ÉÇØ¾ß ÇØ... À¸À¹..."));
				m_fMsgTime += fTimeDelta;
				if (m_fMsgTime >= 2.f)
				{
					pUIManager->Set_MsgCount(1);
					m_fMsgTime = 0.f;
				}
				break;
			case 5:
				pUIManager->Set_MsgName(TEXT("Ä«¸¶µµ ÅºÁö·Î"));
				pUIManager->Set_Msg(TEXT("(Á¤½ÅÀ» ÀÒÀº°Í °°´Ù...)"));
				m_fMsgTime += fTimeDelta;
				if (m_fMsgTime >= 2.f)
				{
					pUIManager->Set_MsgCount(1);
					m_fMsgTime = 0.f;
				}
				break;
			default:
				pUIManager->Set_MsgOff();
				pUIManager->Set_RescueCount(1);
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


HRESULT CGoto::SetUp_ShaderResources()
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

HRESULT CGoto::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Goto"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

void CGoto::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CGoto::Get_GuardHit(_int eType)
{
}

void CGoto::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CGoto::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void CGoto::Play_Scene()
{
}

CGoto * CGoto::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CGoto*	pInstance = new CGoto(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CGoto"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGoto::Clone(void * pArg)
{
	CGameObject*	pInstance = new CGoto(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CGoto"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoto::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

}

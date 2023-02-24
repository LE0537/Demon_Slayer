#include "stdafx.h"
#include "..\Public\Ozaki.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"
#include "SoundMgr.h"

COzaki::COzaki(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

COzaki::COzaki(const COzaki & rhs)
	: CCharacters(rhs)
{
}

HRESULT COzaki::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COzaki::Initialize(void * pArg)
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

void COzaki::Tick(_float fTimeDelta)
{

}

void COzaki::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));


	if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
	{
		if (fDist < 30.f)
		{
			Check_Event();
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			m_pModelCom->Play_Animation(fTimeDelta);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT COzaki::Render()
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

HRESULT COzaki::Render_ShadowDepth()
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



HRESULT COzaki::SetUp_ShaderResources()
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

HRESULT COzaki::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Ozaki"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}
void COzaki::Check_Event()
{
	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if (fDist < 5.f)
	{
		pUIManager->Set_NpcForUI(this);
		m_bInteractionCheck = true;
		if (!m_bMsgEnd)
		{
			if (!m_MsgReset)
				pUIManager->Set_InteractionOn();
		}
		else
			pUIManager->Set_InteractionOff();

		if (!m_bMsgStart && !m_bMsgEnd && pGameInstance->Key_Down(DIK_F))
		{
			pUIManager->Set_InteractionOff();
			m_bMsgStart = true;
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
				pUIManager->Set_MsgName(TEXT("귀살대원 오자키"));
				pUIManager->Set_Msg(TEXT("어,얼른 계급이 높은 사람을 데려와 줘..."));
				if (!m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->PlayVoice(TEXT("Ozaki_Dialog_00.wav"), fVOICE);
					m_bSoundCheck = true;
				}
				break;
			case 1:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 오자키"));
				pUIManager->Set_Msg(TEXT("안 그러면 전부 죽어버리게 될 거야!"));
				if (m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->PlayVoice(TEXT("Ozaki_Dialog_01.wav"), fVOICE);
					m_bSoundCheck = false;
				}
				break;
			case 2:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 오자키"));
				pUIManager->Set_Msg(TEXT("부탁이야...."));
				if (!m_bSoundCheck)
				{
					CSoundMgr::Get_Instance()->PlayVoice(TEXT("Ozaki_Dialog_02.wav"), fVOICE);
					m_bSoundCheck = true;
				}
				break;
			case 3:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("카마도 탄지로"));
				pUIManager->Set_Msg(TEXT("지원 요청을 하고 제가 주변을 살피고 올게요!"));
				pUIManager->Set_QuestCount(1);
				pUIManager->Set_ClearCheck(true, 1);
				pUIManager->Set_RescueCount(2);
				m_bMsgEnd = true;
				break;
			default:
				break;
			}
		}

	}
	else
	{
		pUIManager->Set_NpcForUI(nullptr);
		m_bInteractionCheck = false;

		if (!m_bMsgEnd)
			pUIManager->Set_InteractionOff();
	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}
void COzaki::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void COzaki::Get_GuardHit(_int eType)
{
}

void COzaki::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void COzaki::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void COzaki::Play_Scene()
{
}

COzaki * COzaki::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	COzaki*	pInstance = new COzaki(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : COzaki"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * COzaki::Clone(void * pArg)
{
	CGameObject*	pInstance = new COzaki(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : COzaki"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COzaki::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

}

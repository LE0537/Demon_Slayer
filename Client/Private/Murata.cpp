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
			pUIManager->Set_NPC(this);
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
		if (fDist < 40.f)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			m_pModelCom->Play_Animation(fTimeDelta);
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

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
			//여기에 상호작용 F 유아이 띄우기
		}
		if (!m_bMsgStart && !m_bMsgEnd && pGameInstance->Key_Down(DIK_F))
		{
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
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("너 정말 '계'급 맞아? 엄청 강하자나!"));
				break;
			case 1:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("뭐 아무튼...덕분에 살았어. 구해줘서 고마워."));
				break;
			case 2:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("미안하지만 안쪽에 아직 도망치지 못한 대원들이 있어"));
				break;
			case 3:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("너가 좀 구해줘! 나...? 나는 지원 요청을 하러 가볼게!..."));
				break;
			case 4:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("아참! 안쪽에 아주 강력한 혈귀가 있어 아까 놈하곤 비교도 안될정도로..."));
				break;
			case 5:
				pUIManager->Set_MsgOn();
				pUIManager->Set_MsgName(TEXT("귀살대원 무라타"));
				pUIManager->Set_Msg(TEXT("그녀석을 마주친다면 무조건 도망쳐.'주'급이 오기전까진 절대 상대하지마!"));
				m_bMsgEnd = true;
				break;
			default:
				break;
			}
		}
		
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

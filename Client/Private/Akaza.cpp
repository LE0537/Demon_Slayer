#include "stdafx.h"
#include "..\Public\Akaza.h"
#include "Layer.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "ImGuiManager.h"
#include "AkazaState.h"
#include "AkazaIdleState.h"
#include "AkazaToolState.h"
#include "AkazaHitState.h"
#include "Level_GamePlay.h"
#include "AkazaBattleSTState.h"
#include "AkazaGuardHitState.h"
#include "Effect_Manager.h"
#include "AkazaTakeDownState.h"
#include "AkazaUpperHitState.h"

#include "AkazaHitCinema_Kyoujuro.h"
#include "AkazaHitCinema_Nezuko.h"
#include "AkazaHitCinema_Rui.h"
#include "AkazaHitCinema_Shinobu.h"
#include "AkazaHitCinema_Tanjiro.h"
#include "AkazaAkazaScene.h"
#include "AkazaAiState.h"

#include "Tanjiro.h"
using namespace Akaza;


CAkaza::CAkaza(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CAkaza::CAkaza(const CAkaza & rhs)
	: CCharacters(rhs)
{
}

HRESULT CAkaza::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAkaza::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);
	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	m_pNavigationCom->Set_NaviIndex(tCharacterDesc.iNaviIndex);

	m_tInfo.bSub = tCharacterDesc.bSub;
	m_bChange = tCharacterDesc.bSub;
	if (!m_tInfo.bSub && m_i1p != 11)
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
	else if(m_i1p != 11)
	{
		m_pSubChar = *(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar);
		m_pSubChar->Set_SubChar(this);

	}


	if (m_i1p == 11)
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


	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);

	CAkazaState* pState = new CIdleState();
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);

	m_ePlayerType = CCharacters::PLAYER_TYPE::PLAYER_AKAZA;
	Set_Info();
	return S_OK;
}

void CAkaza::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fEffectStartTime = 0.f;
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if (!pUIManager->Get_StroyEventEnd())
	{
		if (!m_bAiTrue && m_i1p == 11)
		{
			m_fAiTime += fTimeDelta;
			if (m_fAiTime > 18.f)
			{
				m_bAiState = true;
				m_bAiTrue = true;
			}
		}
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();


		if (!dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Get_ADVAkaza())
		{
			if (m_bBattleStart)
			{
				CAkazaState* pState = new CBattleStartState();
				m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
				m_bBattleStart = false;
			}
		}

		if (m_bAiState == true)
		{

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
				HandleInput();
		}

		TickState(fTimeDelta);


		if (m_bSplSkl)
		{
			Check_Spl();
		}

		CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
		if (nullptr == pSocket)
			return;
		_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

		m_pSphereCom->Update(matColl);



		if (m_pAkazaState->Get_AkazaState() == CAkazaState::STATE_JUMP || m_pAkazaState->Get_AkazaState() == CAkazaState::STATE_CHANGE || m_pAkazaState->Get_AkazaState() == CAkazaState::STATE_JUMP_ATTACK)
			m_tInfo.bJump = true;
		else
			m_tInfo.bJump = false;

		if (m_pTransformCom->Get_Jump() == true)
			m_tInfo.bJump = true;
		if (m_pAkazaState != nullptr)
		{
			m_iState = m_pAkazaState->Get_AkazaState();
		}
	}
	else if (pUIManager->Get_StroyEventEnd())
	{
		if (!m_bADV_Dead)
		{
			m_bADV_Dead = true;
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

			_vector vPos = { 56.56f, 0.f, 50.03f, 1.f };

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AkazaLeg"), LEVEL_GAMEPLAY, TEXT("Layer_AkazaBody"), &vPos)))
				return;
			vPos.m128_f32[0] += 5.f;
			vPos.m128_f32[1] += 0.5f;
			vPos.m128_f32[2] += 5.f;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AkazaBody"), LEVEL_GAMEPLAY, TEXT("Layer_AkazaBody"), &vPos)))
				return ;
		
			RELEASE_INSTANCE(CGameInstance);
		}
	}
	RELEASE_INSTANCE(CUI_Manager);
}

void CAkaza::Late_Tick(_float fTimeDelta)
{
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if (!pUIManager->Get_StroyEventEnd())
	{
		LateTickState(fTimeDelta);

		if (m_bSceneRender)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		}

		if (g_bCollBox)
		{
			m_pRendererCom->Add_Debug(m_pSphereCom);
		}
	}
	RELEASE_INSTANCE(CUI_Manager);
}

HRESULT CAkaza::Render()
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

HRESULT CAkaza::Render_ShadowDepth()
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

void CAkaza::Boss_Tick(_float fTimeDelta)
{
	CAkazaAIState* pNewState = (CAkazaAIState*)m_pAkazaState->HandleInput(this);

	if (pNewState)
	{
	
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pNewState);
	}

}

void CAkaza::Play_AkazaScene()
{
	CAkazaState* pState = new CAkazaAkazaScene(CAkazaState::TYPE_START);
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
}

void CAkaza::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CAkazaState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CAkazaState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
}

void CAkaza::HandleInput()
{
	CAkazaState* pNewState = m_pAkazaState->HandleInput(this);

	if (pNewState)
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pNewState);


}

void CAkaza::TickState(_float fTimeDelta)
{
	CAkazaState* pNewState = m_pAkazaState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pNewState);
}

void CAkaza::LateTickState(_float fTimeDelta)
{
	CAkazaState* pNewState = m_pAkazaState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pNewState);
	if (m_pAkazaState->Get_AkazaState() == CAkazaState::STATE_MOVE)
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

HRESULT CAkaza::SetUp_ShaderResources()
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

HRESULT CAkaza::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Akaza"), (CComponent**)&m_pModelCom)))
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

void CAkaza::Set_ShadowLightPos()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_float4 vAt = vPos;

	vPos.x -= 15.f;
	vPos.y += 30.f;
	vPos.z -= 30.f;

	pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW, vPos, vAt);

	RELEASE_INSTANCE(CGameInstance);
}

void CAkaza::Set_Info()
{
	m_tInfo.strName = TEXT("¾ÆÄ«ÀÚ");
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

void CAkaza::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if (m_pAkazaState->Get_AkazaState() == CAkazaState::STATE_HIT)
		m_pModelCom->Reset_Anim(CAkaza::ANIMID::ANIM_HIT);

	CAkazaState* pState = new CHitState(_fPow, _bJumpHit);
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);

}

void CAkaza::Get_GuardHit(_int eType)
{
	CAkazaState* pState;
	if (eType == CAkazaState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CAkaza::ANIMID::ANIM_GUARD_HIT_0);
		pState = new CGuardHitState(CAkazaState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CAkaza::ANIMID::ANIM_GUARD_HIT_1);
		pState = new CGuardHitState(CAkazaState::STATE_TYPE::TYPE_LOOP);
	}
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);


}

void CAkaza::Player_TakeDown(_float _fPow, _bool _bJump)
{
	CAkazaState* pState = new CTakeDownState(_fPow, _bJump);
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
}


void CAkaza::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
	CAkazaState* pState = new CUpperHitState(eHitType, CAkazaState::STATE_TYPE::TYPE_START, fBoundPower, fJumpPower, fKnockBackPower);
	m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
}

void CAkaza::Play_Scene()
{
	CAkazaState* pState = nullptr;

	switch (m_pBattleTarget->Get_PlayerType())
	{
	case Client::CCharacters::PLAYER_TANJIRO:
		pState = new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_START);
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
		break;
	case Client::CCharacters::PLAYER_KYOUJURO:
		pState = new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_START);
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
		break;
	case Client::CCharacters::PLAYER_RUI:
		pState = new CHitCinema_Rui(CHitCinema_Rui::SCENE_START);
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
		break;
	case Client::CCharacters::PLAYER_AKAZA:
		break;
	case Client::CCharacters::PLAYER_NEZUKO:
		pState = new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_START);
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
		break;
	case Client::CCharacters::PLAYER_SHINOBU:
		pState = new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_START);
		m_pAkazaState = m_pAkazaState->ChangeState(this, m_pAkazaState, pState);
		break;
	case Client::CCharacters::PLAYER_END:
		break;
	default:
		break;
	}


}

void CAkaza::Check_Spl()
{
	if (!m_bSplEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO1_HIT, m_pBattleTarget);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bSplEffect = true;
	}

	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("L_Hand_1_P1012_V00_C00_Lct");
	CHierarchyNode*		pSocket2 = m_pModelCom->Get_BonePtr("R_Hand_1_P1012_V00_C00_Lct");
	_float4x4 SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	_float4x4 pParentWorldMatrix = *m_pTransformCom->Get_World4x4Ptr();

	XMStoreFloat4x4(&m_WeaponWorld, (pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&SocketPivotMatrix) * XMLoadFloat4x4(&pParentWorldMatrix)));
	XMStoreFloat4x4(&m_WeaponWorld2, (pSocket2->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&SocketPivotMatrix) * XMLoadFloat4x4(&pParentWorldMatrix)));
}

CAkaza * CAkaza::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAkaza*	pInstance = new CAkaza(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAkaza"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAkaza::Clone(void * pArg)
{
	CGameObject*	pInstance = new CAkaza(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAkaza"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAkaza::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pNavigationCom);

	Safe_Delete(m_pAkazaState);
}

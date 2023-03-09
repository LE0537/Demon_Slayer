#include "stdafx.h"
#include "..\Public\TanjiroTrain_CinemaState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "ImGuiManager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"
#include "Train_Head.h"
using namespace Tanjiro;

CTrain_CinemaState::CTrain_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
	m_bDebug = false;
}

CTrain_CinemaState::CTrain_CinemaState(CINEMASCENE eScene, _float fJumpHeight, _float fJumpTime, _float fSpeed, _float fJumpTimer)
{
	m_eScene = eScene;
	m_fJumpHeight = fJumpHeight;
	m_fJumpTime = fJumpTime;
	m_fJumpSpeed = fSpeed;
	m_fJumpTimer = fJumpTimer;
	m_bDebug = true;
}

CTanjiroState * CTrain_CinemaState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CTrain_CinemaState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance* pGameInstance = nullptr;
	CEffect_Manager* pEffectManger = nullptr;
	pTanjiro->Set_EffectTime(fTimeDelta);

	switch (m_eScene)
	{
	case Client::Tanjiro::CTrain_CinemaState::SCENE_START:
		Jump(pTanjiro, fTimeDelta);

		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_START))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_START);
			return new CTrain_CinemaState(SCENE_0, m_fJumpHeight, m_fJumpTime, m_fJumpSpeed, m_fJumpTimer);
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_0:
		Jump(pTanjiro, fTimeDelta);

		if (m_bNextAnim == true)
			return new CTrain_CinemaState(SCENE_1);

		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_1:
		if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIM_IDLE)
		{
			m_fDelay += fTimeDelta;
		}

		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_1))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_1);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_IDLE, true);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_IDLE, 0.01f);
		}

		if(m_fDelay >= 1.5f)
			return new CTrain_CinemaState(SCENE_2);
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_2:
		Jump(pTanjiro, fTimeDelta);

		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_2))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_2);

			return new CTrain_CinemaState(SCENE_3, m_fJumpHeight, m_fJumpTime, m_fJumpSpeed, m_fJumpTimer);
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_3:
		Jump(pTanjiro, fTimeDelta);

		if (m_bNextAnim == true)
			return new CTrain_CinemaState(SCENE_4);
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_4:

		if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIM_IDLE)
		{
			m_fDelay += fTimeDelta;
		}

		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_4))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_4);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_IDLE, true);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_IDLE, 0.01f);
		
		}

		if (m_fDelay >= 1.5f)
		{
			pGameInstance = GET_INSTANCE(CGameInstance);
			dynamic_cast<CTrain_Head*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Enmu_Neck"))->Get_LayerFront())->Set_ModelNum();
			RELEASE_INSTANCE(CGameInstance);
			return new CTrain_CinemaState(SCENE_5);
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_5:

		if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIM_IDLE)
		{
			m_fDelay += fTimeDelta;
		}

		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_5))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_5);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_IDLE,true);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_IDLE, 0.02f);
		
		}
	
		if (m_fDelay >= 1.5f)
			return new CTrain_CinemaState(SCENE_6);
		
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_6:
		if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIM_IDLE)
		{
			m_fDelay += fTimeDelta;
		}
		m_fTime += fTimeDelta;
		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_6))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_6);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_IDLE, true);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_IDLE, 0.02f);
		}
		if (m_fTime > 0.5f && !m_bTrain_Neck)
		{
			m_bTrain_Neck = true;
			pGameInstance = GET_INSTANCE(CGameInstance);
			dynamic_cast<CTrain_Head*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Enmu_Neck"))->Get_LayerFront())->Set_ModelNum();
			RELEASE_INSTANCE(CGameInstance);
		}
		if (m_fDelay >= 1.5f)
			return new CTrain_CinemaState(SCENE_7);
		
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_7:
		if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIM_IDLE)
		{
			m_fDelay += fTimeDelta;
		}
		if(m_bTrue)
			m_fTime += fTimeDelta;
		if (pTanjiro->Get_Model()->Get_End(CTrain_CinemaState::ANIM_SCENE_7))
		{
			pTanjiro->Get_Model()->Set_End(CTrain_CinemaState::ANIM_SCENE_7);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_IDLE, true);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_IDLE, 0.02f);

		}
		if (g_bDeathTime && m_fTime > 1.f)
		{
			g_bDeathTime = false;
			pTanjiro->Set_BossEnmu_Dead(false);
			pTanjiro->Get_Transform()->Turn2(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_RIGHT), -120.f);
		}
		else if (m_fTime > 2.f)
		{
			if (!m_bEffect2)
			{

				pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPHERE_MAIN1, pTanjiro);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPHERE_ATC1, pTanjiro);

				RELEASE_INSTANCE(CEffect_Manager);
				m_bEffect2 = true;
			}
		}
		//return new CIdleState();
		

		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


	if (!m_bSlow && m_eScene == CTrain_CinemaState::SCENE_7)
	{
		g_bDeathTime = true;
		m_bSlow = true;
	}
	else if (m_eScene == CTrain_CinemaState::SCENE_7)
	{

	}
	return nullptr;
}

CTanjiroState * CTrain_CinemaState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance* pGameInstance = nullptr;
	CEffect_Manager* pEffectManger = nullptr;
	switch (m_eScene)
	{
	case Client::Tanjiro::CTrain_CinemaState::SCENE_START:
		if (!m_bEffect)
		{
			pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_2:
		if (!m_bEffect)
		{
			pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_5:
		if (!m_bEffect)
		{
		

			pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOCOMMON1, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOCOMMON2, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_6:
		if (!m_bEffect)
		{
		

			pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOMOVE1, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOMOVE2, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOMOVE3, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOMOVE4, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOMOVE5, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_7:
		if (!m_bEffect)
		{
			pEffectManger = GET_INSTANCE(CEffect_Manager);

			//pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPHERE_MAIN1, pTanjiro);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOSPHERE_ATC2, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
			m_bTrue = true;
		}
		break;
	default:
		break;
	}

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CTrain_CinemaState::Enter(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = nullptr;

	switch (m_eScene)
	{
	case Client::Tanjiro::CTrain_CinemaState::SCENE_START:
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.4582f, 16.609f, 180.9894f, 1.f));
		Initialize_JumpState(pTanjiro, XMVectorSet(7.011f, 27.244f, 167.110f, 1.f));
	
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_START);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_START));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_START, 0.01f);

		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_0:
		Initialize_JumpState(pTanjiro, XMVectorSet(7.011f, 27.244f, 167.110f, 1.f));
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_0);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_0));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_0, true);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_0, 0.01f);
	
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_1);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_1));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_2:
		Initialize_JumpState(pTanjiro, XMVectorSet(-0.153f, 5.795f, 159.317f, 1.f));
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_2);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_2));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_2, 0.01f);
	
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_3:
		Initialize_JumpState(pTanjiro, XMVectorSet(-0.153f, 5.795f, 159.317f, 1.f));
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_3);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_3));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_3, true);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_4:
		pTanjiro->Get_Transform()->Set_PlayerLookAt(XMVectorSet(0.063f, 8.350f, 152.388f, 1.f));
		//pTanjiro->Get_Transform()->Set_RotationY(90.f);
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_4);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_4));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_4);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_4);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_4, 0.01f);
		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_5:
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_5);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_5));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_5);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_5);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_5, 0.01f);

		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_6:
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_6);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_6));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_6);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_6);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_6, 0.2f);

		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_7:
		pTanjiro->Get_Model()->Reset_Anim(CTrain_CinemaState::ANIM_SCENE_7);
		pTanjiro->Get_Model()->Set_CurrentTime_Index(CTrain_CinemaState::ANIM_SCENE_7, 10.f);
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.072f, 8.2f, 156.468f, 1.f));
		//pTanjiro->Get_Transform()->LookAt(XMVectorSet(0.072f, 5.942f, 156.468f, 1.f));
		pTanjiro->Get_Transform()->Turn2(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_RIGHT), 90.f);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID> (CTrain_CinemaState::ANIM_SCENE_7));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTrain_CinemaState::ANIM_SCENE_7);
		pTanjiro->Get_Model()->Set_Loop(CTrain_CinemaState::ANIM_SCENE_7);
		pTanjiro->Get_Model()->Set_LinearTime(CTrain_CinemaState::ANIM_SCENE_7, 0.1f);

		break;
	case Client::Tanjiro::CTrain_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CTrain_CinemaState::Exit(CTanjiro * pTanjiro)
{

}

void CTrain_CinemaState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (m_fJumpTimer <= m_fJumpTime)
	{
		m_fJumpTimer += fTimeDelta; // deltaTime은 이전 프레임에서 현재 프레임까지 걸린 시간

		_float fTime = m_fJumpTimer / m_fJumpTime;

		XMFLOAT3 jumpStartPos = m_vPlayerPosition;
		jumpStartPos.y += m_fJumpHeight;
		XMFLOAT3 jumpEndPos = m_vTargetPosition;
		jumpEndPos.y -= m_fJumpHeight;
		XMVECTOR start = XMLoadFloat3(&jumpStartPos);
		XMVECTOR end = XMLoadFloat3(&jumpEndPos);
		XMVECTOR posVec = XMVectorLerp(start, end, fTime);
		XMStoreFloat3(&m_vPlayerPosition, posVec);
		_vector vPosition = XMLoadFloat3(&m_vPlayerPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	}
	else
	{
		m_vPlayerPosition = m_vTargetPosition;
		_vector vPosition = XMLoadFloat3(&m_vPlayerPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_bNextAnim = true;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	if (m_vTargetPosition.y <= XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
	{
		m_bMaxHeight = true;
	}

	if (m_vTargetPosition.y >= XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)) && m_bMaxHeight)
	{
		m_vPlayerPosition = m_vTargetPosition;
		_vector vPosition = XMLoadFloat3(&m_vPlayerPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_bNextAnim = true;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}



}

void CTrain_CinemaState::Initialize_JumpState(CTanjiro * pTanjiro, _fvector vTargetPosition)
{
	//m_fJumpTimer = 0.0f;
	//float jumpStartHeight = pTanjiro->Get_NavigationHeight().y;
	float jumpStartHeight = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	float jumpEndHeight = XMVectorGetY(vTargetPosition) + m_fJumpHeight;
	XMFLOAT3 jumpStartPos;  XMStoreFloat3(&jumpStartPos, pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	jumpStartPos.y = jumpStartHeight;
	XMFLOAT3 jumpEndPos; XMStoreFloat3(&jumpEndPos, vTargetPosition);
	jumpEndPos.y = jumpEndHeight;
	XMVECTOR start = XMLoadFloat3(&jumpStartPos);
	XMVECTOR end = XMLoadFloat3(&jumpEndPos);
	XMVECTOR jumpVec = XMVectorSubtract(end, start);
	m_fJumpHeight = XMVectorGetY(jumpVec) / m_fJumpSpeed; // 50.0f는 플레이어의 점프 속도

	XMStoreFloat3(&m_vPlayerPosition, pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	XMStoreFloat3(&m_vTargetPosition, vTargetPosition);

}




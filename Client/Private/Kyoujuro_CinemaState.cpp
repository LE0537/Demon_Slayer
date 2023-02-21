#include "stdafx.h"
#include "..\Public\Kyoujuro_CinemaState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;

CKyoujuro_CinemaState::CKyoujuro_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CKyoujuro_CinemaState::HandleInput(CKyoujuro * pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_START:
		Scene_Start(pKyoujuro, fTimeDelta);

		if (m_bNextAnim == true)
			return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::SCENE_0);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_0:
		Scene_0(pKyoujuro, fTimeDelta);

		if (m_bNextAnim == true)
			return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::SCENE_1);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_1:
		Scene_1(pKyoujuro, fTimeDelta);

		if (m_bNextAnim == true)
			return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::SCENE_2);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_2:
		m_bAnimStop = true;
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 0.5f);

		if (pKyoujuro->Get_Model()->Get_CurrentTime_Index(pKyoujuro->Get_AnimIndex()) >= 13.f)
		{
			
			return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::SCENE_3);
		}

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_3:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_3))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_3);
			return new CIdleState();
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_4:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_6:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	if (m_bAnimStop == false)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CKyoujuro_CinemaState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_SKILL_COMMON;

	pKyoujuro->Set_SplSkl(true);

	switch (m_eScene)
	{
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_START_0);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_START_0);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_START_0));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_START_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_START_0, 0.01f);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pKyoujuro->Get_NavigationHeight().y, 38.5f, 1.f));

		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_0));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_0, true);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_0, 0.01f);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_1:

		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_1));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_2:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_2));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_2, 0.05f);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_3:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_3));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_3, 0.05f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_4:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_6:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


}

void CKyoujuro_CinemaState::Exit(CKyoujuro * pKyoujuro)
{
}

CKyoujuroState * CKyoujuro_CinemaState::Scene_Start(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_START_0))
	{
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_START_1);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_START_1);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_START_1));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_START_1, true);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_START_1, 0.01f);


		pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_START_0);
	}


	if (pKyoujuro->Get_AnimIndex() == CKyoujuro_CinemaState::ANIM_SCENE_START_1)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= 1.5f)
			m_bNextAnim = true;
	}




	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Scene_0(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	m_vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(m_vTargetPosition - m_vMyPosition));

	if (fDistance >= 8.f)
	{
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * 2.f, pKyoujuro->Get_NavigationCom());
	}
	else
		m_bNextAnim = true;



	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Scene_1(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	m_vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(m_vTargetPosition - m_vMyPosition));

	if (fDistance >= 5.f && m_bAnimStop == false && m_bIsHitMotion == false)
	{
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * 2.f, pKyoujuro->Get_NavigationCom());
	}


	if (pKyoujuro->Get_Model()->Get_CurrentTime_Index(pKyoujuro->Get_AnimIndex()) >= 15.f)
	{
		if (m_bControlMotion == false)
		{
			pKyoujuro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bControlMotion = true;
		}
	}


	if (pKyoujuro->Get_Model()->Get_CurrentTime_Index(pKyoujuro->Get_AnimIndex()) >= 20.f)
	{

		if (m_bIsHitMotion == false)
		{
			g_bDeathTime = true;
			//pKyoujuro->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_KNOCKBACK, 0.f, 0.f, 5.f);
			m_bAnimStop = true;
			m_bIsHitMotion = true;
		}

		if(m_bAnimStop == true)
			pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 0.08f);

		m_fTime += (1.f/60.f);

		if (m_fTime >= 1.5f)
		{
			m_bAnimStop = false;
			g_bDeathTime = false;
			m_fTime = 0.f;
		}
	}

	if (pKyoujuro->Get_Model()->Get_CurrentTime_Index(pKyoujuro->Get_AnimIndex()) >= 22.f)
	{
		if (m_bMotionDelay == false)
		{
			pKyoujuro->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_KNOCKBACK, 20.f, 30.f, 7.f);
			m_bMotionDelay = true;
		}
	}


	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		m_bNextAnim = true;

		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	

	return nullptr;
}

void CKyoujuro_CinemaState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
}

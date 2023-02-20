#include "stdafx.h"
#include "..\Public\Shinobu_CinemaState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"


using namespace Shinobu;

CShinobu_CinemaState::CShinobu_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CShinobu_CinemaState::HandleInput(CShinobu * pShinobu)
{
	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		Scene_Start(pShinobu, fTimeDelta);

		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_START))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_START);
			return new CShinobu_CinemaState(CShinobu_CinemaState::SCENE_0);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_4:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_5:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_6:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Late_Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	if (m_bAnimStop == false)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CShinobu_CinemaState::Enter(CShinobu * pShinobu)
{
	m_eStateId = STATE_SKILL_COMMON;

	pShinobu->Set_SplSkl(true);

	//ÅºÁö·Î 57.932f, 1.f, 43.087f
	//	»ó´ë 59.838f, 0.f, 45.601f

	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_START));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_START, 0.02f);

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(57.932f, pShinobu->Get_NavigationHeight().y, 43.087f, 1.f));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(59.838f, pShinobu->Get_NavigationHeight().y, 45.601f, 1.f));

		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_4:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_5:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_6:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CShinobu_CinemaState::Exit(CShinobu * pShinobu)
{
}

CShinobuState * CShinobu_CinemaState::Scene_Start(CShinobu * pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_Model()->Get_CurrentTime_Index(pShinobu->Get_AnimIndex()) >= 30.f)
	{
		if (m_bControlMotion == false)
		{
			pShinobu->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_KNOCKBACK, 5.f, 10.f, 7.f);
			m_bControlMotion = true;
		}
	}
	else if (pShinobu->Get_Model()->Get_CurrentTime_Index(pShinobu->Get_AnimIndex()) < 30.f)
	{
		if (m_bIsHitMotion == false)
		{
			pShinobu->Get_BattleTarget()->Take_Damage(0.0f, false);
			m_bIsHitMotion = true;
		}

		m_fTime += fTimeDelta;

		if (m_fTime >= 0.3f)
		{
			m_bIsHitMotion = false;
			m_fTime = 0.f;
		}
	}



	return nullptr;
}

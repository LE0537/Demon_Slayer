#include "stdafx.h"
#include "..\Public\Akaza_CinemaState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"

using namespace Akaza;

CAkaza_CinemaState::CAkaza_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CAkaza_CinemaState::HandleInput(CAkaza * pAkaza)
{


	return nullptr;
}

CAkazaState * CAkaza_CinemaState::Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Akaza::CAkaza_CinemaState::SCENE_START:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_START))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_START);
			return new CAkaza_CinemaState(SCENE_0);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_0:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_0))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_0);
			return new CAkaza_CinemaState(SCENE_1);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_1:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_1))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_1);
			return new CAkaza_CinemaState(SCENE_2);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_2:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_2))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_2);
			return new CAkaza_CinemaState(SCENE_3);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_3:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_3))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_3);
			return new CAkaza_CinemaState(SCENE_4);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_4:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_4))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_4);
			return new CAkaza_CinemaState(SCENE_5);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_5:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_5))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_5);
			return new CAkaza_CinemaState(SCENE_6);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_6:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_6))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_6);
			return new CAkaza_CinemaState(SCENE_7);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_7:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_7))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_7);
			return new CIdleState();
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CAkazaState * CAkaza_CinemaState::Late_Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CAkaza_CinemaState::Enter(CAkaza * pAkaza)
{
	switch (m_eScene)
	{
	case Client::Akaza::CAkaza_CinemaState::SCENE_START:
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.2f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_START);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_START);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_START));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_START);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_0:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_0);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_0);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_0));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_1:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_1);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_1);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_1));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_2:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_2);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_2);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_2));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_3:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_3);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_3);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_3));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_3);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_4:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_060);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_4);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_4);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_4));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_4);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_4, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_5:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_070);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_5);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_5);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_5));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_5);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_5, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_6:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_080);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_6);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_6);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_6));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_6);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_6, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_7:
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_090);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_7);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_7);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_7));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_7);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_7, 0.01f);
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_8:
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CAkaza_CinemaState::Exit(CAkaza * pAkaza)
{
}

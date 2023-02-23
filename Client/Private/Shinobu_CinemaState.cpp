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
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_START))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_START);
			return new CShinobu_CinemaState(SCENE_0);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_0))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_0);
			return new CShinobu_CinemaState(SCENE_1);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_1))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_1);
			return new CShinobu_CinemaState(SCENE_2);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_2))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_2);
			return new CShinobu_CinemaState(SCENE_3);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_3))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_3);
			return new CIdleState();
		}
		break;

	default:
		break;
	}


	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Late_Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	
	pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CShinobu_CinemaState::Enter(CShinobu * pShinobu)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(62.66f, pShinobu->Get_NavigationHeight().y, 47.29f, 1.f));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(62.86f, pShinobu->Get_NavigationHeight().y, 47.29f, 1.f));
		pShinobu->Get_Transform()->Set_PlayerLookAt(XMVectorSet(56.46f, pShinobu->Get_NavigationHeight().y, 56.46f, 1.f));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(56.46f, pShinobu->Get_NavigationHeight().y, 56.46f, 1.f));
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_START));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_0);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_0);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_0));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_1);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_1);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_1));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_2);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_2);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_2));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_3);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_3);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_3));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_3);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_3, 0.01f);
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


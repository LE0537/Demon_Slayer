#include "stdafx.h"
#include "AkazaHitCinema_Shinobu.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"


using namespace Akaza;


CHitCinema_Shinobu::CHitCinema_Shinobu(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CHitCinema_Shinobu::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CHitCinema_Shinobu::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Shinobu::SCENE_START:
		if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_0);
		}
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_0:
		if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_1);
		}
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_1:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
			return new CIdleState();
		}
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}

	return nullptr;

}

CAkazaState * CHitCinema_Shinobu::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Shinobu::Enter(CAkaza* pAkaza)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Shinobu::SCENE_START:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_010));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_0:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_030));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_1:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_050));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}
}

void CHitCinema_Shinobu::Exit(CAkaza* pAkaza)
{
}

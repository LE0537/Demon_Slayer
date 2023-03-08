#include "stdafx.h"
#include "NezukoHitCinema_Shinobu.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"


using namespace Nezuko;


CHitCinema_Shinobu::CHitCinema_Shinobu(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CHitCinema_Shinobu::HandleInput(CNezuko * pNezuko)
{
	return nullptr;
}

CNezukoState * CHitCinema_Shinobu::Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_START:
		if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_0);
		}
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_0:
		if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_1);
		}
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_1:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
			return new CIdleState();
		}
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}

	return nullptr;

}

CNezukoState * CHitCinema_Shinobu::Late_Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Shinobu::Enter(CNezuko * pNezuko)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_START:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_010));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_0:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_030));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_1:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_050));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}
}

void CHitCinema_Shinobu::Exit(CNezuko * pNezuko)
{
	pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
}

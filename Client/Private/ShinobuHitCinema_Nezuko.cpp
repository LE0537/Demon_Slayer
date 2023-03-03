#include "stdafx.h"
#include "ShinobuHitCinema_Nezuko.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"

using namespace Shinobu;

CHitCinema_Nezuko::CHitCinema_Nezuko(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CHitCinema_Nezuko::HandleInput(CShinobu * pShinobu)
{
	return nullptr;
}

CShinobuState * CHitCinema_Nezuko::Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_START:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);

			if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_0);
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_0:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_1);
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_1:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_2);
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_2:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_3);
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_3:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_4);
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_4:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_100)
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_5);
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_5:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_110)
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_6);
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_6:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
			return new CIdleState();
		}
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_7:
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CShinobuState * CHitCinema_Nezuko::Late_Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Nezuko::Enter(CShinobu * pShinobu)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_010));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_0:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_030));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_1:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_040));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_2:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_050));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_3:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_060));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_4:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_080));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_5:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_100));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_100, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_6:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_110));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_110, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Nezuko::Exit(CShinobu * pShinobu)
{
}

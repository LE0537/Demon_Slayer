#include "stdafx.h"
#include "AkazaHitCinema_Nezuko.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"

using namespace Akaza;

CHitCinema_Nezuko::CHitCinema_Nezuko(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CHitCinema_Nezuko::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CHitCinema_Nezuko::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Nezuko::SCENE_START:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_0);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_0:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_1);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_1:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_2);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_2:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_3);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_3:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_4);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_4:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_100)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_5);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_5:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);

			if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_110)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_6);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_6:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
			return new CIdleState();
		}
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_7:
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CAkazaState * CHitCinema_Nezuko::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Nezuko::Enter(CAkaza* pAkaza)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Nezuko::SCENE_START:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_010));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_0:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_030));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_1:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_040));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_2:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_050));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_3:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_060));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_4:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_080));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_5:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_100));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_100, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_6:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_110));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_110, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Nezuko::Exit(CAkaza* pAkaza)
{
}

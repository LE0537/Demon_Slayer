#include "stdafx.h"
#include "TanjiroHitCinema_Nezuko.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

CHitCinema_Nezuko::CHitCinema_Nezuko(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CTanjiroState * CHitCinema_Nezuko::HandleInput(CTanjiro* pTanjiro)
{
	return nullptr;
}

CTanjiroState * CHitCinema_Nezuko::Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_START:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_0);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_0:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_1);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_1:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_2);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_2:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_3);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_3:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_4);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_4:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_100)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_5);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_5:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);

			if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_110)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_6);
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_6:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
			return new CIdleState();
		}
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_7:
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CTanjiroState * CHitCinema_Nezuko::Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Nezuko::Enter(CTanjiro* pTanjiro)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_START:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_010));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_030));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_040));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_2:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_050));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_3:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_060));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_4:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_080));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_5:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_100));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_100, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_6:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_110));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_110, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Nezuko::Exit(CTanjiro* pTanjiro)
{
}

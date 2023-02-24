#include "stdafx.h"
#include "NezukoHitCinema_Akaza.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"

using namespace Nezuko;

CHitCinema_Akaza::CHitCinema_Akaza(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CHitCinema_Akaza::HandleInput(CNezuko* pNezuko)
{
	

	return nullptr;
}

CNezukoState * CHitCinema_Akaza::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Akaza::SCENE_START:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010);

			if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_0);
			}
		}
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_0:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050);

			if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_1);
			}
		}
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_1:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080);

			if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_2);
			}
		}
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_2:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);

			//if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CIdleState();
			}
		}
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CNezukoState * CHitCinema_Akaza::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Akaza::Enter(CNezuko* pNezuko)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Akaza::SCENE_START:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_010));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_0:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_050));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_1:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_080));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_2:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_090));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_090, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Akaza::Exit(CNezuko* pNezuko)
{
}

#include "stdafx.h"
#include "AkazaHitCinema_Rui.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"

using namespace Akaza;


CHitCinema_Rui::CHitCinema_Rui(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CHitCinema_Rui::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CHitCinema_Rui::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Rui::SCENE_START:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_010))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_010);

			//if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_0);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_0:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_030))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_030);

		//	if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_1);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_1:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_040))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_040);

		//	if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_2);
			}
		}
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_2:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_060))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_060);

		//	if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_3);
			}
		}

		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_3:
		if (pAkaza->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_070))
		{
			pAkaza->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_070);

			return new CIdleState();
		}
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CAkazaState * CHitCinema_Rui::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if(m_bAnimStop == false)
		pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Rui::Enter(CAkaza* pAkaza)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Rui::SCENE_START:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_010));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_0:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_030));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_1:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_040));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_2:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_060));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_3:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_070));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Rui::Exit(CAkaza* pAkaza)
{
}

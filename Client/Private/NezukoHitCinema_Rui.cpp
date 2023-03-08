#include "stdafx.h"
#include "NezukoHitCinema_Rui.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"

using namespace Nezuko;


CHitCinema_Rui::CHitCinema_Rui(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CHitCinema_Rui::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CHitCinema_Rui::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Rui::SCENE_START:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_010))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_010);

			//if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_0);
			}
		}
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_0:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_030))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_030);

		//	if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_1);
			}
		}
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_1:
		m_fDuration += fTimeDelta;
		if (5.46f < m_fDuration)
			return new CHitCinema_Rui(CHitCinema_Rui::SCENE_2);
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_2:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_060))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_060);

		//	if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_3);
			}
		}

		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_3:
		if (pNezuko->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_070))
		{
			pNezuko->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_070);

			return new CIdleState();
		}
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CNezukoState * CHitCinema_Rui::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if(m_bAnimStop == false)
		pNezuko->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Rui::Enter(CNezuko* pNezuko)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Rui::SCENE_START:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_010));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_0:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_030));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_1:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_040));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_2:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_060));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_3:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_070));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Rui::Exit(CNezuko* pNezuko)
{
	pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
}

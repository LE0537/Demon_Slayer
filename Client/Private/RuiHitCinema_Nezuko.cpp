#include "stdafx.h"
#include "RuiHitCinema_Nezuko.h"
#include "GameInstance.h"
#include "RuiIdleState.h"

using namespace Rui;

CHitCinema_Nezuko::CHitCinema_Nezuko(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CHitCinema_Nezuko::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CHitCinema_Nezuko::Tick(CRui* pRui, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Nezuko::SCENE_START:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_0);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_0:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_1);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_1:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_2);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_2:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_3);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_3:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_4);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_4:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_100)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_5);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_5:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);

			if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_110)
			{
				return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_6);
			}
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_6:
		if (pRui->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
			return new CIdleState();
		}
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_7:
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CRuiState * CHitCinema_Nezuko::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Nezuko::Enter(CRui* pRui)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Nezuko::SCENE_START:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_010));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_0:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_030));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_1:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_040));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_2:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_050));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_3:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_060));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_4:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_080));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_5:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_100));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_100, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_6:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_110));
		pRui->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_110, 0.01f);
		break;
	case Client::Rui::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Nezuko::Exit(CRui* pRui)
{
}

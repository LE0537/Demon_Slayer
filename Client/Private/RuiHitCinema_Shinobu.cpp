#include "stdafx.h"
#include "RuiHitCinema_Shinobu.h"
#include "GameInstance.h"
#include "RuiIdleState.h"


using namespace Rui;


CHitCinema_Shinobu::CHitCinema_Shinobu(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CHitCinema_Shinobu::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CHitCinema_Shinobu::Tick(CRui* pRui, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Shinobu::SCENE_START:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
		{
			pRui->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_0);
		}
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_0:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pRui->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_1);
		}
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_1:
		if (pRui->Get_Model()->Get_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050))
		{
			pRui->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
			return new CIdleState();
		}
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}

	return nullptr;

}

CRuiState * CHitCinema_Shinobu::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Shinobu::Enter(CRui* pRui)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Shinobu::SCENE_START:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_010));
		pRui->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_0:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_030));
		pRui->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_1:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_050));
		pRui->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}
}

void CHitCinema_Shinobu::Exit(CRui* pRui)
{
	pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
}

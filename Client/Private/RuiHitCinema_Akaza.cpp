#include "stdafx.h"
#include "RuiHitCinema_Akaza.h"
#include "GameInstance.h"
#include "RuiIdleState.h"

using namespace Rui;

CHitCinema_Akaza::CHitCinema_Akaza(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CHitCinema_Akaza::HandleInput(CRui* pRui)
{


	return nullptr;
}

CRuiState * CHitCinema_Akaza::Tick(CRui* pRui, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Akaza::SCENE_START:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_0);
		}
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_0:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_1);
		}
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_1:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			pRui->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
			return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_2);
		}
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_2:
		if (pRui->Get_BattleTarget()->Get_TargetState() == 0)
		{
			pRui->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
			return new CIdleState();
		}
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CRuiState * CHitCinema_Akaza::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Akaza::Enter(CRui* pRui)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Akaza::SCENE_START:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_010));
		pRui->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_0:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_050));
		pRui->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_1:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_080));
		pRui->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_2:
		pRui->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pRui->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_090));
		pRui->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_090, 0.01f);
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Akaza::Exit(CRui* pRui)
{
}

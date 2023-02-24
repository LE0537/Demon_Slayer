#include "stdafx.h"
#include "RuiHitCinema_Kyoujuro.h"
#include "GameInstance.h"
#include "RuiIdleState.h"

using namespace Rui;

CHitCinema_Kyoujuro::CHitCinema_Kyoujuro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CHitCinema_Kyoujuro::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CHitCinema_Kyoujuro::Tick(CRui* pRui, _float fTimeDelta)
{


	int i = pRui->Get_BattleTarget()->Get_SkillType();
	int a = 0;
	printf("%d \n", i);

	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_START:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
			{
				pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
			}
		}
			//return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
		//{
		//	pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//}
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_0:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
		{
			if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
			{
				pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		}

		//if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
		//{
		//	pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		//}
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_1:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
			{
				pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		}
		//if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
		//{
		//	pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		//}
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_2:
		if (pRui->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
			{
				pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
				return new CIdleState();
			}
			//return new CIdleState();
		}
		//if (pRui->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
		//{
		//	pRui->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		//	return new CIdleState();
		//}
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CRuiState * CHitCinema_Kyoujuro::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CHitCinema_Kyoujuro::Enter(CRui* pRui)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_START:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010));
		pRui->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_0:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060));
		pRui->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_1:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070));
		pRui->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_2:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075));
		pRui->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075, 0.01f);
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}



}

void CHitCinema_Kyoujuro::Exit(CRui* pRui)
{
}

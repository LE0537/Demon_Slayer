#include "stdafx.h"
#include "ShinobuHitCinema_Rui.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"

using namespace Shinobu;


CHitCinema_Rui::CHitCinema_Rui(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CHitCinema_Rui::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CHitCinema_Rui::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Rui::SCENE_START:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_010))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_010);

			//if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_0);
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_0:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_030))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_030);

		//	if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_1);
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_1:
		m_fDuration += fTimeDelta;
		if(5.46f < m_fDuration)
			return new CHitCinema_Rui(CHitCinema_Rui::SCENE_2);			
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_2:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_060))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_060);

		//	if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_3);
			}
		}

		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_3:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_070))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_070);

			return new CIdleState();
		}
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CShinobuState * CHitCinema_Rui::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if(m_bAnimStop == false)
		pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Rui::Enter(CShinobu* pShinobu)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Rui::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_010));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_0:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_030));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_1:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_040));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_2:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_060));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_3:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_070));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Rui::Exit(CShinobu* pShinobu)
{
}

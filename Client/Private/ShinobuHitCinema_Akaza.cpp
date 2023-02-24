#include "stdafx.h"
#include "ShinobuHitCinema_Akaza.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"

using namespace Shinobu;

CHitCinema_Akaza::CHitCinema_Akaza(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CHitCinema_Akaza::HandleInput(CShinobu* pShinobu)
{
	

	return nullptr;
}

CShinobuState * CHitCinema_Akaza::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Akaza::SCENE_START:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010);

			if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_0);
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_0:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050);

			if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_1);
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_1:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080);

			if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_2);
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_2:
		if (pShinobu->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090))
		{
			pShinobu->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);

			//if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CIdleState();
			}
		}
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CShinobuState * CHitCinema_Akaza::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Akaza::Enter(CShinobu* pShinobu)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Akaza::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_010));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_0:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_050));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_1:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_080));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_2:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_090));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_090, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Akaza::Exit(CShinobu* pShinobu)
{
}

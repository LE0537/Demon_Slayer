#include "stdafx.h"
#include "TanjiroHitCinema_Shinobu.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"


using namespace Tanjiro;


CHitCinema_Shinobu::CHitCinema_Shinobu(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CTanjiroState * CHitCinema_Shinobu::HandleInput(CTanjiro* pTanjiro)
{
	return nullptr;
}

CTanjiroState * CHitCinema_Shinobu::Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_START:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_0);
		}
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_0:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_1);
		}
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_1:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
			return new CIdleState();
		}
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}

	return nullptr;

}

CTanjiroState * CHitCinema_Shinobu::Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Shinobu::Enter(CTanjiro* pTanjiro)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_START:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_010));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_030));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_050));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Tanjiro::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}
}

void CHitCinema_Shinobu::Exit(CTanjiro* pTanjiro)
{
}

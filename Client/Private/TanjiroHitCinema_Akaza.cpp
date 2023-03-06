#include "stdafx.h"
#include "TanjiroHitCinema_Akaza.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

CHitCinema_Akaza::CHitCinema_Akaza(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CTanjiroState * CHitCinema_Akaza::HandleInput(CTanjiro* pTanjiro)
{


	return nullptr;
}

CTanjiroState * CHitCinema_Akaza::Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_START:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
			return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_0);
		}
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_0:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
			return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_1);
		}
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_1:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
			return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_2);
		}
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_2:
		if (pTanjiro->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090))
		{
			pTanjiro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);

			//if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CIdleState();
			}
		}
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CTanjiroState * CHitCinema_Akaza::Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Akaza::Enter(CTanjiro* pTanjiro)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_START:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_010));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_050));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_080));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_2:
		pTanjiro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_090));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_090, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Tanjiro::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Akaza::Exit(CTanjiro* pTanjiro)
{
}

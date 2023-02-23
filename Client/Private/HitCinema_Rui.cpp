#include "stdafx.h"
#include "..\Public\HitCinema_Rui.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;


HitCinema_Rui::HitCinema_Rui(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CTanjiroState * HitCinema_Rui::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * HitCinema_Rui::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	int i = pTanjiro->Get_BattleTarget()->Get_SkillType();
	printf("SkillType : %d \n", i);
	switch (m_eScene)
	{
	case Client::Tanjiro::HitCinema_Rui::SCENE_START:
		if (pTanjiro->Get_Model()->Get_End(HitCinema_Rui::ANIM_SCENE_DMG_010))
		{
			pTanjiro->Get_Model()->Set_End(HitCinema_Rui::ANIM_SCENE_DMG_010);

			//if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new HitCinema_Rui(HitCinema_Rui::SCENE_0);
			}
		}
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_0:
		if (pTanjiro->Get_Model()->Get_End(HitCinema_Rui::ANIM_SCENE_DMG_030))
		{
			pTanjiro->Get_Model()->Set_End(HitCinema_Rui::ANIM_SCENE_DMG_030);

		//	if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new HitCinema_Rui(HitCinema_Rui::SCENE_1);
			}
		}
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_1:
		if (pTanjiro->Get_Model()->Get_End(HitCinema_Rui::ANIM_SCENE_DMG_040))
		{
			pTanjiro->Get_Model()->Set_End(HitCinema_Rui::ANIM_SCENE_DMG_040);

		//	if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
			{
				return new HitCinema_Rui(HitCinema_Rui::SCENE_2);
			}
		}
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_2:
		if (pTanjiro->Get_Model()->Get_End(HitCinema_Rui::ANIM_SCENE_DMG_060))
		{
			pTanjiro->Get_Model()->Set_End(HitCinema_Rui::ANIM_SCENE_DMG_060);

		//	if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
			{
				return new HitCinema_Rui(HitCinema_Rui::SCENE_3);
			}
		}

		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_3:
		if (pTanjiro->Get_Model()->Get_End(HitCinema_Rui::ANIM_SCENE_DMG_070))
		{
			pTanjiro->Get_Model()->Set_End(HitCinema_Rui::ANIM_SCENE_DMG_070);

			return new CIdleState();
		}
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_4:
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_5:
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_6:
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CTanjiroState * HitCinema_Rui::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if(m_bAnimStop == false)
		pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void HitCinema_Rui::Enter(CTanjiro * pTanjiro)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::HitCinema_Rui::SCENE_START:
		pTanjiro->Get_Model()->Reset_Anim(HitCinema_Rui::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(HitCinema_Rui::ANIM_SCENE_DMG_010);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(HitCinema_Rui::ANIM_SCENE_DMG_010));
		pTanjiro->Get_Model()->Set_Loop(HitCinema_Rui::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_LinearTime(HitCinema_Rui::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(HitCinema_Rui::ANIM_SCENE_DMG_030);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(HitCinema_Rui::ANIM_SCENE_DMG_030);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(HitCinema_Rui::ANIM_SCENE_DMG_030));
		pTanjiro->Get_Model()->Set_Loop(HitCinema_Rui::ANIM_SCENE_DMG_030);
		pTanjiro->Get_Model()->Set_LinearTime(HitCinema_Rui::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(HitCinema_Rui::ANIM_SCENE_DMG_040);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(HitCinema_Rui::ANIM_SCENE_DMG_040);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(HitCinema_Rui::ANIM_SCENE_DMG_040));
		pTanjiro->Get_Model()->Set_Loop(HitCinema_Rui::ANIM_SCENE_DMG_040);
		pTanjiro->Get_Model()->Set_LinearTime(HitCinema_Rui::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_2:
		pTanjiro->Get_Model()->Reset_Anim(HitCinema_Rui::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(HitCinema_Rui::ANIM_SCENE_DMG_060);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(HitCinema_Rui::ANIM_SCENE_DMG_060));
		pTanjiro->Get_Model()->Set_Loop(HitCinema_Rui::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Set_LinearTime(HitCinema_Rui::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_3:
		pTanjiro->Get_Model()->Reset_Anim(HitCinema_Rui::ANIM_SCENE_DMG_070);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(HitCinema_Rui::ANIM_SCENE_DMG_070);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(HitCinema_Rui::ANIM_SCENE_DMG_070));
		pTanjiro->Get_Model()->Set_Loop(HitCinema_Rui::ANIM_SCENE_DMG_070);
		pTanjiro->Get_Model()->Set_LinearTime(HitCinema_Rui::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_4:
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_5:
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_6:
		break;
	case Client::Tanjiro::HitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}

}

void HitCinema_Rui::Exit(CTanjiro * pTanjiro)
{
}

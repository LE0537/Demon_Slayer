#include "stdafx.h"
#include "TanjiroHitCinema_Kyoujuro.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

CHitCinema_Kyoujuro::CHitCinema_Kyoujuro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CTanjiroState * CHitCinema_Kyoujuro::HandleInput(CTanjiro* pTanjiro)
{
	return nullptr;
}

CTanjiroState * CHitCinema_Kyoujuro::Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{


	int i = pTanjiro->Get_BattleTarget()->Get_SkillType();
	int a = 0;
	printf("%d \n", i);

	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_START:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
			{
				pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
			}
		}
			//return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
		//{
		//	pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//}
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_0:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
		{
			if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
			{
				pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		}

		//if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
		//{
		//	pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		//}
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_1:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
			{
				pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		}
		//if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
		//{
		//	pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		//}
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_2:
		if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
			{
				pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
				return new CIdleState();
			}
			//return new CIdleState();
		}
		//if (pTanjiro->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
		//{
		//	pTanjiro->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		//	return new CIdleState();
		//}
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CTanjiroState * CHitCinema_Kyoujuro::Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CHitCinema_Kyoujuro::Enter(CTanjiro* pTanjiro)
{
	switch (m_eScene)
	{
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_START:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_2:
		pTanjiro->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075));
		pTanjiro->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pTanjiro->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075, 0.01f);
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Tanjiro::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}



}

void CHitCinema_Kyoujuro::Exit(CTanjiro* pTanjiro)
{
	pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
}

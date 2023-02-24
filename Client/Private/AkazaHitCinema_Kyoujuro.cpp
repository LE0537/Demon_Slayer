#include "stdafx.h"
#include "AkazaHitCinema_Kyoujuro.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"

using namespace Akaza;

CHitCinema_Kyoujuro::CHitCinema_Kyoujuro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CHitCinema_Kyoujuro::HandleInput(CAkaza * pAkaza)
{
	return nullptr;
}

CAkazaState * CHitCinema_Kyoujuro::Tick(CAkaza * pAkaza, _float fTimeDelta)
{


	int i = pAkaza->Get_BattleTarget()->Get_SkillType();
	int a = 0;
	printf("%d \n", i);

	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_START:
		if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
			{
				pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
			}
		}
			//return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
		//{
		//	pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//}
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_0:
		if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
		{
			if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
			{
				pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		}

		//if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
		//{
		//	pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		//}
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_1:
		if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
			{
				pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		}
		//if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
		//{
		//	pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		//}
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_2:
		if (pAkaza->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
			{
				pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
				return new CIdleState();
			}
			//return new CIdleState();
		}
		//if (pAkaza->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
		//{
		//	pAkaza->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		//	return new CIdleState();
		//}
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CAkazaState * CHitCinema_Kyoujuro::Late_Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CHitCinema_Kyoujuro::Enter(CAkaza * pAkaza)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_START:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_0:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_1:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_2:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}



}

void CHitCinema_Kyoujuro::Exit(CAkaza * pAkaza)
{
}

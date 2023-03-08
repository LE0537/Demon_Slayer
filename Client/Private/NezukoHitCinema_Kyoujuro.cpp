#include "stdafx.h"
#include "NezukoHitCinema_Kyoujuro.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"

using namespace Nezuko;

CHitCinema_Kyoujuro::CHitCinema_Kyoujuro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CHitCinema_Kyoujuro::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CHitCinema_Kyoujuro::Tick(CNezuko* pNezuko, _float fTimeDelta)
{


	int i = pNezuko->Get_BattleTarget()->Get_SkillType();
	int a = 0;
	printf("%d \n", i);

	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_START:
		if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
			{
				pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
			}
		}
			//return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
		//{
		//	pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//}
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_0:
		if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
		{
			if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
			{
				pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		}

		//if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
		//{
		//	pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		//}
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_1:
		if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
			{
				pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		}
		//if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
		//{
		//	pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		//}
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_2:
		if (pNezuko->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
			{
				pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
				return new CIdleState();
			}
			//return new CIdleState();
		}
		//if (pNezuko->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
		//{
		//	pNezuko->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		//	return new CIdleState();
		//}
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CNezukoState * CHitCinema_Kyoujuro::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CHitCinema_Kyoujuro::Enter(CNezuko* pNezuko)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_START:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_0:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_1:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_2:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}



}

void CHitCinema_Kyoujuro::Exit(CNezuko* pNezuko)
{
	pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
}

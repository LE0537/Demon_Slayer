#include "stdafx.h"
#include "ShinobuHitCinema_Kyoujuro.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"

using namespace Shinobu;

CHitCinema_Kyoujuro::CHitCinema_Kyoujuro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CHitCinema_Kyoujuro::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CHitCinema_Kyoujuro::Tick(CShinobu* pShinobu, _float fTimeDelta)
{


	int i = pShinobu->Get_BattleTarget()->Get_SkillType();
	int a = 0;
	printf("%d \n", i);

	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_START:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
			{
				pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
			}
		}
			//return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010))
		//{
		//	pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_0);
		//}
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_0:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
		{
			if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
			{
				pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		}

		//if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060))
		//{
		//	pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_1);
		//}
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_1:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
			{
				pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
				return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
			}
			return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		}
		//if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070))
		//{
		//	pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		//	return new CHitCinema_Kyoujuro(CHitCinema_Kyoujuro::SCENE_2);
		//}
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_2:
		if (pShinobu->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
		{
			if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
			{
				pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
				return new CIdleState();
			}
			//return new CIdleState();
		}
		//if (pShinobu->Get_Model()->Get_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075))
		//{
		//	pShinobu->Get_Model()->Set_End(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		//	return new CIdleState();
		//}
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CShinobuState * CHitCinema_Kyoujuro::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CHitCinema_Kyoujuro::Enter(CShinobu* pShinobu)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_0:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_1:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_2:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_070);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Kyoujuro::ANIM_SCENE_DMG_075, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_3:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Kyoujuro::SCENE_END:
		break;
	default:
		break;
	}



}

void CHitCinema_Kyoujuro::Exit(CShinobu* pShinobu)
{
}

#include "stdafx.h"
#include "KyoujuroHitCinema_Rui.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;


CHitCinema_Rui::CHitCinema_Rui(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CHitCinema_Rui::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CHitCinema_Rui::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_START:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_010))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_010);

			//if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_0);
			}
		}
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_0:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_030))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_030);

		//	if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_1);
			}
		}
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_1:
		m_fDuration += fTimeDelta;
		if (5.46f < m_fDuration)
			return new CHitCinema_Rui(CHitCinema_Rui::SCENE_2);
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_2:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_060))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_060);

		//	if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_070)
			{
				return new CHitCinema_Rui(CHitCinema_Rui::SCENE_3);
			}
		}

		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_3:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Rui::ANIM_SCENE_DMG_070))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Rui::ANIM_SCENE_DMG_070);

			return new CIdleState();
		}
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CKyoujuroState * CHitCinema_Rui::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if(m_bAnimStop == false)
		pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Rui::Enter(CKyoujuro* pKyoujuro)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_010));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_030));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_030);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_1:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_040));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_040);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_2:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_060));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_060);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_3:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Rui::ANIM_SCENE_DMG_070));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Rui::ANIM_SCENE_DMG_070);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Rui::ANIM_SCENE_DMG_070, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Rui::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Rui::Exit(CKyoujuro* pKyoujuro)
{
	pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
}

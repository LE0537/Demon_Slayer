#include "stdafx.h"
#include "KyoujuroHitCinema_Shinobu.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"


using namespace Kyoujuro;


CHitCinema_Shinobu::CHitCinema_Shinobu(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CHitCinema_Shinobu::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CHitCinema_Shinobu::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_START:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_0);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_0:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
			return new CHitCinema_Shinobu(CHitCinema_Shinobu::SCENE_1);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_1:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
			return new CIdleState();
		}
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}

	return nullptr;

}

CKyoujuroState * CHitCinema_Shinobu::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Shinobu::Enter(CKyoujuro* pKyoujuro)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_010));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_030));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_030);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_1:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Shinobu::ANIM_SCENE_DMG_050));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Shinobu::ANIM_SCENE_DMG_050);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Shinobu::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_2:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_3:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Shinobu::SCENE_END:
		break;
	default:
		break;
	}
}

void CHitCinema_Shinobu::Exit(CKyoujuro* pKyoujuro)
{
	pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
}

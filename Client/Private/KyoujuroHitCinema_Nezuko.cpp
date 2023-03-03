#include "stdafx.h"
#include "KyoujuroHitCinema_Nezuko.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;

CHitCinema_Nezuko::CHitCinema_Nezuko(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CHitCinema_Nezuko::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CHitCinema_Nezuko::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_START:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_0);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_0:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_040)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_1);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_1:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_2);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_2:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_060)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_3);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_3:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_4);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_4:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_100)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_5);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_5:
		if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_110)
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
			return new CHitCinema_Nezuko(CHitCinema_Nezuko::SCENE_6);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_6:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
			return new CIdleState();
		}
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_7:
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CKyoujuroState * CHitCinema_Nezuko::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Nezuko::Enter(CKyoujuro* pKyoujuro)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_010));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_030));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_030);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_030, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_1:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_040));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_040);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_040, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_2:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_050));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_050);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_3:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_060));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_060);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_060, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_4:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_080));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_080);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_5:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_100));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_100);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_100, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_6:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Nezuko::ANIM_SCENE_DMG_110));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Nezuko::ANIM_SCENE_DMG_110);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Nezuko::ANIM_SCENE_DMG_110, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Nezuko::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Nezuko::Exit(CKyoujuro* pKyoujuro)
{
}

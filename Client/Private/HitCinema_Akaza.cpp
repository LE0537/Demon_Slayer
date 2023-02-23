#include "stdafx.h"
#include "HitCinema_Akaza.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;

CHitCinema_Akaza::CHitCinema_Akaza(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CHitCinema_Akaza::HandleInput(CKyoujuro * pKyoujuro)
{
	

	return nullptr;
}

CKyoujuroState * CHitCinema_Akaza::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_START:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_010);

			if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_050)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_0);
			}
		}
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_0:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_050);

			if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_080)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_1);
			}
		}
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_1:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_080);

			if (pKyoujuro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_090)
			{
				return new CHitCinema_Akaza(CHitCinema_Akaza::SCENE_2);
			}
		}
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_2:
		if (pKyoujuro->Get_Model()->Get_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090))
		{
			pKyoujuro->Get_Model()->Set_End(CHitCinema_Akaza::ANIM_SCENE_DMG_090);

			//if (pTanjiro->Get_BattleTarget()->Get_SkillType() == CCharacters::SKILL_TYPE::SKILL_030)
			{
				return new CIdleState();
			}
		}
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CKyoujuroState * CHitCinema_Akaza::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Akaza::Enter(CKyoujuro * pKyoujuro)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_010));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_010);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_010, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_050));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_050);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_050, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_1:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_080));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_080);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_080, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_2:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Akaza::ANIM_SCENE_DMG_090));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Akaza::ANIM_SCENE_DMG_090);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Akaza::ANIM_SCENE_DMG_090, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_3:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Akaza::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Akaza::Exit(CKyoujuro * pKyoujuro)
{
}

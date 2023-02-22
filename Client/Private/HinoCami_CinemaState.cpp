#include "stdafx.h"
#include "..\Public\HinoCami_CinemaState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

CHinoCami_CinemaState::CHinoCami_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;

}

CTanjiroState * CHinoCami_CinemaState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
		switch (m_eScene)
		{
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_START:
			if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_START))
			{
				pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_START);
				return new CHinoCami_CinemaState(SCENE_0);
			}
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
			if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_0))
			{
				pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_0);
				return new CHinoCami_CinemaState(SCENE_1);
			}
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1:
			if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_1))
			{
				pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_1);
				return new CHinoCami_CinemaState(SCENE_2);
			}
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2:
			if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_2))
			{
				pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_2);
				return new CHinoCami_CinemaState(SCENE_3);
			}
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3:
			if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_3))
			{
				pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_3);
				return new CIdleState();
			}
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_4:
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_5:
			break;
		case Client::Tanjiro::CHinoCami_CinemaState::SCENE_END:
			break;
		default:
			break;
		}



	

	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta * 0.2f);

	return nullptr;
}

void CHinoCami_CinemaState::Enter(CTanjiro * pTanjiro)
{
	
	switch (m_eScene)
	{
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_START:
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pTanjiro->Get_NavigationHeight().y, 64.f, 1.f));
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1:
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2:
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3:
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_4:
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_5:
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CHinoCami_CinemaState::Exit(CTanjiro * pTanjiro)
{

}


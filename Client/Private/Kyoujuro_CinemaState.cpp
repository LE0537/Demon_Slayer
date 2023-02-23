#include "stdafx.h"
#include "..\Public\Kyoujuro_CinemaState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;

CKyoujuro_CinemaState::CKyoujuro_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CKyoujuro_CinemaState::HandleInput(CKyoujuro * pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	

	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{


	pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CKyoujuro_CinemaState::Enter(CKyoujuro * pKyoujuro)
{


	switch (m_eScene)
	{
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_START);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_START);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_START));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_START, 0.01f);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pKyoujuro->Get_NavigationHeight().y, 38.5f, 1.f));

		//pKyoujuro->Get_Transform()->Set_PlayerLookAt();
		//pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt();
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_0));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_0, true);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_0, 0.01f);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_1:

		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_1));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_2:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_2));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_2, 0.05f);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_3:
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_3));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_3, 0.05f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_4:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_6:
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


}

void CKyoujuro_CinemaState::Exit(CKyoujuro * pKyoujuro)
{
}


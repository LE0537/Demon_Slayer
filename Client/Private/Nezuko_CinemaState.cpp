#include "stdafx.h"
#include "..\Public\Nezuko_CinemaState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"

using namespace Nezuko;

CNezuko_CinemaState::CNezuko_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CNezuko_CinemaState::HandleInput(CNezuko * pNezuko)
{
	return nullptr;
}

CNezukoState * CNezuko_CinemaState::Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CNezuko_CinemaState::SCENE_START:
		Scene_Start(pNezuko, fTimeDelta);

		if (m_bNextAnim == true)
			return new CNezuko_CinemaState(CNezuko_CinemaState::SCENE_0);
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_0:


		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_1:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_2:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_3:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_4:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_5:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_6:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CNezukoState * CNezuko_CinemaState::Late_Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	if (m_bAnimStop == false)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CNezuko_CinemaState::Enter(CNezuko * pNezuko)
{
	m_eStateId = STATE_SKILL_COMMON;

	switch (m_eScene)
	{
	case Client::Nezuko::CNezuko_CinemaState::SCENE_START:
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_START);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_START);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_START));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_START);
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_START_RUN, true);

		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_START, 0.01f);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_START_RUN, 0.01f);

		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pNezuko->Get_NavigationHeight().y, 64.f, 1.f));
		pNezuko->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pNezuko->Get_NavigationHeight().y, 38.5f, 1.f));

		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pNezuko->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_0:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_1:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_2:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_3:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_4:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_5:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_6:
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_END:
		break;
	default:
		break;
	}
	


}

void CNezuko_CinemaState::Exit(CNezuko * pNezuko)
{

}

CNezukoState * CNezuko_CinemaState::Scene_Start(CNezuko * pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_AnimIndex() == CNezuko_CinemaState::ANIM_SCENE_START)
	{
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_START))
		{
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_START_RUN);
			pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_START_RUN));
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_START);
		}

	}

	if (pNezuko->Get_AnimIndex() == CNezuko_CinemaState::ANIM_SCENE_START_RUN)
	{
		_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

		if (fDistance >= 7.f)
		{
			pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 2.f, pNezuko->Get_NavigationCom());
			m_bNextAnim = true;
		}
	}



	return nullptr;
}

CNezukoState * CNezuko_CinemaState::Scene_0(CNezuko * pNezuko, _float fTimeDelta)
{
	return nullptr;
}

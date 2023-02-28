#include "stdafx.h"
#include "..\Public\NezukoSplSkrStartState.h"
#include "GameInstance.h"
#include "Nezuko_CinemaState.h"
#include "NezukoIdleState.h"

using namespace Nezuko;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CSplSkrStartState::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CSplSkrStartState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_DEFAULT);
			break;
		case Client::CNezukoState::TYPE_DEFAULT:
			if (m_bCollision == true)
			{
				pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
				pNezuko->Get_BattleTarget()->Play_Scene();
				return new CNezuko_CinemaState(CNezuko_CinemaState::SCENE_START);
			}
			else
			{
				pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CNezukoState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		break;
	case Client::CNezukoState::TYPE_LOOP:
		break;
	case Client::CNezukoState::TYPE_END:
		m_fDuration += fTimeDelta;

		if (m_fDuration <= 0.7f)
			Move(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CNezukoState * CSplSkrStartState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_0);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_0, 0.1f);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_1, 0.1f);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_2);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_2, 0.1f);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_3);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_3);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_3);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_3, 0.1f);
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CNezuko* pNezuko)
{
}

void CSplSkrStartState::Move(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}

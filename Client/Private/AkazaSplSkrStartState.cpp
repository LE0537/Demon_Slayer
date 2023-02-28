#include "stdafx.h"
#include "..\Public\AkazaSplSkrStartState.h"
#include "GameInstance.h"
#include "Akaza_CinemaState.h"
#include "AkazaIdleState.h"

using namespace Akaza;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CSplSkrStartState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CSplSkrStartState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				pAkaza->Get_BattleTarget()->Play_Scene();
				return new CAkaza_CinemaState(CAkaza_CinemaState::SCENE_START);
			}
			else
			{
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CAkazaState::TYPE_END:
			break;
		case Client::CAkazaState::TYPE_DEFAULT:
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		break;
	case Client::CAkazaState::TYPE_LOOP:
		if (pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIMID::ANIM_SPLSKL_START_1) <= 10.f && pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIMID::ANIM_SPLSKL_START_1) >= 2.f)
		{
			m_fDuration += fTimeDelta;

			if (m_fDuration <= 0.5f)
				Move(pAkaza, fTimeDelta);
		}
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CAkazaState * CSplSkrStartState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SPLSKL_START_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SPLSKL_START_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_SPLSKL_START_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SPLSKL_START_0, 0.1f);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SPLSKL_START_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SPLSKL_START_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_SPLSKL_START_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SPLSKL_START_1, 0.1f);
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CAkaza* pAkaza)
{
}

void CSplSkrStartState::Move(CAkaza* pAkaza, _float fTimeDelta)
{
	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
	pAkaza->Get_Transform()->Go_Straight(fTimeDelta * 2.2f);
}

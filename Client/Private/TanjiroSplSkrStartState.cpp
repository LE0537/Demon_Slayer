#include "stdafx.h"
#include "..\Public\TanjiroSplSkrStartState.h"
#include "GameInstance.h"
#include "HinoCami_CinemaState.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CSplSkrStartState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CSplSkrStartState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CHinoCami_CinemaState(CHinoCami_CinemaState::SCENE_START);
			}
			else
			{
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CTanjiroState::TYPE_END:
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			break;
		case Client::CTanjiroState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		m_fDuration += fTimeDelta;

		if(m_fDuration <= 0.7f)
			Move(pTanjiro, fTimeDelta);
		break;
	case Client::CTanjiroState::TYPE_END:
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	case Client::CTanjiroState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CTanjiroState * CSplSkrStartState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SPLSKL_START_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SPLSKL_START_0);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_SPLSKL_START_0);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SPLSKL_START_0, 0.1f);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SPLSKL_START_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SPLSKL_START_1);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_SPLSKL_START_1);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SPLSKL_START_1, 0.1f);
		break;
	case Client::CTanjiroState::TYPE_END:
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	case Client::CTanjiroState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CTanjiro * pTanjiro)
{
}

void CSplSkrStartState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if(fDistance >= 5.f)
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}

#include "stdafx.h"
#include "..\Public\KyoujuroSplSkrStartState.h"
#include "GameInstance.h"
#include "Kyoujuro_CinemaState.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CKyoujuroState * CSplSkrStartState::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CSplSkrStartState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_BattleTarget()->Play_Scene();
				return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::SCENE_START);
			}
			else
			{
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CKyoujuroState::TYPE_END:
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			break;
		case Client::CKyoujuroState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		m_fDuration += fTimeDelta;

		if(m_fDuration <= 0.7f)
			Move(pKyoujuro, fTimeDelta);
		break;
	case Client::CKyoujuroState::TYPE_END:
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CKyoujuroState * CSplSkrStartState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SPLSKL_START_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_SPLSKL_START_0);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SPLSKL_START_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_SPLSKL_START_0, 0.1f);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SPLSKL_START_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_SPLSKL_START_1);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SPLSKL_START_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_SPLSKL_START_1, 0.1f);
		break;
	case Client::CKyoujuroState::TYPE_END:
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CKyoujuro* pKyoujuro)
{
}

void CSplSkrStartState::Move(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}

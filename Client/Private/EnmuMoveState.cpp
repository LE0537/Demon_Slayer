#include "stdafx.h"
#include "EnmuIdlestate.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "..\Public\EnmuMoveState.h"

using namespace Enmu;

CMoveState::CMoveState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CMoveState::HandleInput(CEnmu * pEnmu)
{
	return nullptr;
}

CEnmuState * CMoveState::Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			return new CMoveState(TYPE_LOOP);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			break;
		case Client::CEnmuState::TYPE_END:
			break;
		case Client::CEnmuState::TYPE_DEFAULT:
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	if (m_eStateType == TYPE_LOOP)
	{
		Move(pEnmu, fTimeDelta);

		if (m_bNextAnim == true)
			return new CIdleState();

	}


	return nullptr;
}

CEnmuState * CMoveState::Late_Tick(CEnmu * pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;

}

void CMoveState::Enter(CEnmu * pEnmu)
{
	m_eStateId = CEnmuState::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_MOVE_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_MOVE_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_MOVE_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_MOVE_0, 0.05f);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_MOVE_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_MOVE_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIM_MOVE_1);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_MOVE_1, 0.05f);
		break;
	case Client::CEnmuState::TYPE_END:
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CMoveState::Exit(CEnmu * pEnmu)
{
}

void CMoveState::Move(CEnmu * pEnmu, _float fTimeDelta)
{
	m_fMoveTime += fTimeDelta;

	_vector vEnmuPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vEnmuPosition));


	if (fDistance <= 5.f || m_fMoveTime >= 1.3f)
	{
		m_bNextAnim = true;
	}
	else
	{
		pEnmu->Get_Transform()->Set_PlayerLookAt(vTargetPosition);
		pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 1.3f, pEnmu->Get_NavigationCom());
	}



}

#include "stdafx.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "RuiDadMoveState.h"
#include "RuiDadAtk_1_State.h"
#include "RuiDadJumpState.h"
#include "RuiDadGuardState.h"
#include "RuiDadTargetRushState.h"

using namespace RuiDad;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CRuiDadState * CIdleState::HandleInput(CRuiDad* pRuiDad)
{
	pRuiDad->Set_bGuard(false);
	

	return nullptr;
}

CRuiDadState * CIdleState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	
	return nullptr;
}

CRuiDadState * CIdleState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	if (vPlayerY.m128_f32[1] > 0.f)
	{
		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	}
	if (pRuiDad->Get_PlayerInfo().iGuard < pRuiDad->Get_PlayerInfo().iMaxGuard)
	{
		pRuiDad->Set_GuardHp(1);
		if (pRuiDad->Get_PlayerInfo().iGuard > pRuiDad->Get_PlayerInfo().iMaxGuard)
			pRuiDad->Set_ResetGuardHp();
	}
	return nullptr;
}

void CIdleState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_IDLE);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_IDLE);
}

void CIdleState::Exit(CRuiDad* pRuiDad)
{
}

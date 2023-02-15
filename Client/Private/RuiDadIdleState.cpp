#include "stdafx.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "RuiDadMoveState.h"
#include "RuiDadAtk_1_State.h"
#include "RuiDadJumpState.h"
#include "RuiDadGuardState.h"
#include "RuiDadTargetRushState.h"
#include "RuiDadSkill_JumpDrop.h"
#include "RuiDadSkill_Rush.h"
using namespace RuiDad;

CIdleState::CIdleState()
{
}

CRuiDadState * CIdleState::HandleInput(CRuiDad* pRuiDad)
{
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(DIK_M))
		return new CSkill_JumpDropState(STATE_TYPE::TYPE_START);


	if (pGameInstance->Key_Down(DIK_P))
		return new CSkill_RushState(STATE_TYPE::TYPE_START);


	//switch (m_eRange)
	//{
	//case Client::RuiDad::CIdleState::RANGE_NEAR:
	//	Update_AI_Near(pRuiDad);
	//	break;
	//case Client::RuiDad::CIdleState::RANGE_FAR:
	//	Update_AI_Far(pRuiDad);
	//	break;
	//case Client::RuiDad::CIdleState::RANGE_OUT:
	//	Update_AI_Out(pRuiDad);
	//	break;
	//}

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
	pRuiDad->Set_bGuard(false);
	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_IDLE);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_IDLE);
}

void CIdleState::Exit(CRuiDad* pRuiDad)
{
}

void CIdleState::Update_TargetState(CRuiDad * pRuiDad)
{
	CCharacters* pTarget = pRuiDad->Get_BattleTarget();

	m_iTargetState = pTarget->Get_TargetState();

	switch (m_iTargetState)
	{
	case 0:
		m_eTargetState = TARGET_STATE::STATE_IDLE;
		break;
	case 1:
		m_eTargetState = TARGET_STATE::STATE_MOVE;
		break;
	case 2:
		m_eTargetState = TARGET_STATE::STATE_JUMP;
		break;
	case 3: case 4: case 5: case 6:
		m_eTargetState = TARGET_STATE::STATE_ATTACK;
		break;
	case 7:
		m_eTargetState = TARGET_STATE::STATE_HIT;
		break;
	case 8: case 9: case 10:
		m_eTargetState = TARGET_STATE::STATE_GUARD;
		break;
	case 11:
		m_eTargetState = TARGET_STATE::STATE_MOVE;
		break;
	case 12: case 13: case 14: case 15: case 16: case 17:
		m_eTargetState = TARGET_STATE::STATE_SKILL;
		break;
	case 19:
		m_eTargetState = TARGET_STATE::STATE_ATTACK;
		break;
	case 21:
		m_eTargetState = TARGET_STATE::STATE_RUSH;
		break;
	}

	_vector vTargetPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance <= 30.f) { m_eRange = RANGE_IN; }
	else { m_eRange = RANGE_OUT; }


	if (m_eRange == RANGE_IN)
	{
		if (fDistance <= 15.f) { m_eRange = RANGE_NEAR; }
		else { m_eRange = RANGE_FAR; }
	}


}

void CIdleState::Update_TargetToAngle(CRuiDad * pRuiDad)
{
}

void CIdleState::Update_AI_Near(CRuiDad * pRuiDad)
{
}

void CIdleState::Update_AI_Far(CRuiDad * pRuiDad)
{
}

void CIdleState::Update_AI_Out(CRuiDad * pRuiDad)
{
}

CRuiDadState * CIdleState::Return_AIState(CRuiDad * pRuiDad)
{
	return nullptr;
}

void CIdleState::Update_NearAttack()
{
}

void CIdleState::Update_NearMove()
{
}

void CIdleState::Update_NearGuard()
{
}

void CIdleState::Update_NearRush()
{
}

void CIdleState::Update_NearSkill()
{
}

_bool CIdleState::Far_CompareOriginPoint(CRuiDad * pRuiDad)
{
	return _bool();
}

void CIdleState::Update_Far_InAttack()
{
}

void CIdleState::Update_Far_InMove()
{
}

void CIdleState::Update_Far_InGuard()
{
}

void CIdleState::Update_Far_InRush()
{
}

void CIdleState::Update_Far_InSkill()
{
}

void CIdleState::Update_Far_OutAttack()
{
}

void CIdleState::Update_Far_OutMove()
{
}

void CIdleState::Update_Far_OutGuard()
{
}

void CIdleState::Update_Far_OutRush()
{
}

void CIdleState::Update_Far_OutSkill()
{
}

void CIdleState::Near_Skill_Setting(CRuiDad * pRuiDad)
{
}

void CIdleState::Dash_Setting(CRuiDad * pRuiDad)
{
}

void CIdleState::DashDir_Calcul(CRuiDad * pRuiDad)
{
}

void CIdleState::Compare_OriginPoint(CRuiDad * pRuiDad)
{
}

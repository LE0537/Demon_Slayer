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
#include "RuiDadSkill_Throw.h"
#include "RuiDadSkill_Punch.h"
#include "RuiDadDashState.h"
#include "Tanjiro.h"
using namespace RuiDad;

CIdleState::CIdleState()
{
}

CRuiDadState * CIdleState::HandleInput(CRuiDad* pRuiDad)
{
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	if (pRuiDad->Get_RuiDadAiMode() == true)
	{
		Update_TargetState(pRuiDad);


		switch (m_eRange)
		{
		case Client::RuiDad::CIdleState::RANGE_NEAR:
			Update_AI_Near(pRuiDad);
			break;
		case Client::RuiDad::CIdleState::RANGE_FAR:
			Update_AI_Far(pRuiDad);
			break;
		case Client::RuiDad::CIdleState::RANGE_OUT:
			Update_AI_Out(pRuiDad);
			break;
		}

		if (pRuiDad->Get_RuiDadHit() == true)
			m_ePreState = AI_HIT;
		else
			m_ePreState = m_eState;


		if (m_ePreState == AI_HIT)
		{
			if (m_fDelay <= 0.3f)
			{
				return nullptr;
			}
			else
			{
				pRuiDad->Set_RuiDadHit(false);
				m_fDelay = 0.f;
				return  Return_AIState(pRuiDad);
			}
		}
		else
			return  Return_AIState(pRuiDad);
	}
	else
	{
		if (pRuiDad->Get_QuestStop() == false && dynamic_cast<CTanjiro*>(pRuiDad->Get_BattleTarget())->Get_Quest2())
			return new CMoveState(OBJDIR::DIR_STRAIGHT, TYPE_START);
	}


	return nullptr;
}

CRuiDadState * CIdleState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	if (m_ePreState == AI_HIT)
		m_fDelay += fTimeDelta;


	return nullptr;
}

CRuiDadState * CIdleState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CIdleState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_IDLE;
	pRuiDad->Set_bGuard(false);
	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_IDLE);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_IDLE);
	pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex(),true);
	pRuiDad->Get_Model()->Set_LinearTime(pRuiDad->Get_AnimIndex(), 0.05f);

	pRuiDad->Set_RuiDadHit(false);
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

	if (fDistance <= 20.f) { m_eRange = RANGE_IN; }
	else { m_eRange = RANGE_OUT; }


	if (m_eRange == RANGE_IN)
	{
		if (fDistance <= 10.f) { m_eRange = RANGE_NEAR; }
		else { m_eRange = RANGE_FAR; }
	}


}

void CIdleState::Update_TargetToAngle(CRuiDad * pRuiDad)
{
	CCharacters* pTarget = pRuiDad->Get_BattleTarget();
	_vector vTargetPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vRuiLook = XMVector3Normalize(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector vTargetLook = XMVector3Normalize(vTargetPosition - vMyPosition);

	_vector DotRuiTarget = XMVector3AngleBetweenVectors(vRuiLook, vTargetLook);
	_float fAngle = XMConvertToDegrees(XMVectorGetX(DotRuiTarget));

	//pRui->Get_Transform()->Set_RotationY(fAngle);

	pRuiDad->Get_Transform()->Set_PlayerLookAt(vTargetPosition);
}

void CIdleState::Update_AI_Near(CRuiDad * pRuiDad)
{
	// TARGET STATE 검사


	switch (m_eTargetState)
	{
	case Client::RuiDad::CIdleState::STATE_IDLE:
	case Client::RuiDad::CIdleState::STATE_MOVE:
	case Client::RuiDad::CIdleState::STATE_JUMP:
	case Client::RuiDad::CIdleState::STATE_DASH:
		Update_NearMove();
		break;
	case Client::RuiDad::CIdleState::STATE_ATTACK:
		Update_NearAttack();
		break;
	case Client::RuiDad::CIdleState::STATE_SKILL:
		Update_NearSkill();
		break;
	case Client::RuiDad::CIdleState::STATE_GUARD:
		Update_NearGuard();
		break;
	case Client::RuiDad::CIdleState::STATE_HIT:
		break;
	case Client::RuiDad::CIdleState::STATE_RUSH:
		Update_NearRush();
		break;
	case Client::RuiDad::CIdleState::STATE_END:
		break;
	default:
		break;
	}
}

void CIdleState::Update_AI_Far(CRuiDad * pRuiDad)
{
	if (Far_CompareOriginPoint(pRuiDad) == true)  // 루이가 원래 지점과 가까울 때
	{
		switch (m_eTargetState)
		{
		case Client::RuiDad::CIdleState::STATE_IDLE:
		case Client::RuiDad::CIdleState::STATE_MOVE:
		case Client::RuiDad::CIdleState::STATE_JUMP:
		case Client::RuiDad::CIdleState::STATE_DASH:
			Update_Far_InMove();
			break;
		case Client::RuiDad::CIdleState::STATE_ATTACK:
			Update_Far_InAttack();
			break;
		case Client::RuiDad::CIdleState::STATE_SKILL:
			Update_Far_InSkill();
			break;
		case Client::RuiDad::CIdleState::STATE_GUARD:
			Update_Far_InGuard();
			break;
		case Client::RuiDad::CIdleState::STATE_HIT:
			break;
		case Client::RuiDad::CIdleState::STATE_RUSH:
			Update_Far_InRush();
			break;
		case Client::RuiDad::CIdleState::STATE_END:
			break;
		default:
			break;
		}
	}
	else // 루이가 원래 지점과 멀리 떨어져 있을 경우
	{
		switch (m_eTargetState)
		{
		case Client::RuiDad::CIdleState::STATE_IDLE:
		case Client::RuiDad::CIdleState::STATE_MOVE:
		case Client::RuiDad::CIdleState::STATE_JUMP:
		case Client::RuiDad::CIdleState::STATE_DASH:
			Update_Far_OutMove();
			break;
		case Client::RuiDad::CIdleState::STATE_ATTACK:
			Update_Far_OutAttack();
			break;
		case Client::RuiDad::CIdleState::STATE_SKILL:
			Update_Far_OutSkill();
			break;
		case Client::RuiDad::CIdleState::STATE_GUARD:
			Update_Far_OutGuard();
			break;
		case Client::RuiDad::CIdleState::STATE_HIT:
			break;
		case Client::RuiDad::CIdleState::STATE_RUSH:
			Update_Far_OutRush();
			break;
		case Client::RuiDad::CIdleState::STATE_END:
			break;
		default:
			break;
		}
	}
}

void CIdleState::Update_AI_Out(CRuiDad * pRuiDad)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL;
		break;
	case 2:
		m_eState = AI_SKILL;
		break;
	case 3:
		m_eState = AI_SKILL;
		break;
	case 4:
		m_eState = AI_SKILL;
		break;
	case 5:
		m_eState = AI_SKILL;
		break;
	case 6:
		m_eState = AI_IDLE;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_RUSH;
		break;
	case 9:
		m_eState = AI_FRONTMOVE;
		break;
	case 10:
		m_eState = AI_DASH_F;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	}
}

CRuiDadState * CIdleState::Return_AIState(CRuiDad * pRuiDad)
{
	// 최종 AI 스테이트
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(0, 1);
	int iRandom = RandomPattern(gen);


	if (m_eState == AI_STATE::AI_DASH)
		Dash_Setting(pRuiDad);
	else if (m_eState == AI_STATE::AI_SKILL && m_eRange == RuiDad::CIdleState::RANGE_NEAR)
		Near_Skill_Setting(pRuiDad);

	switch (m_eState)
	{
	case Client::RuiDad::CIdleState::AI_IDLE:
		Enter(pRuiDad);
		return nullptr;
		break;
	case Client::RuiDad::CIdleState::AI_BACKMOVE:
		return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		break;
	
	case Client::RuiDad::CIdleState::AI_FRONTMOVE:
		return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_ATTACK:
		return new CAtk_1_State(TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_GUARD:
		return new CGuardState(STATE_TYPE::TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_SKILL:
		break;
	case Client::RuiDad::CIdleState::AI_SKILL_0:
		return new CSkill_PunchState(TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_SKILL_1:
		return new CSkill_ThrowState(TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_SKILL_2:
		return new CSkill_RushState(TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_SKILL_3:
		return new CSkill_JumpDropState(TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_DASH:
		break;
	case Client::RuiDad::CIdleState::AI_DASH_F:
		return new CDashState(OBJDIR::DIR_STRAIGHT, false, false);
		break;
	case Client::RuiDad::CIdleState::AI_DASH_B:
		return new CDashState(OBJDIR::DIR_BACK, false, false);
		break;
	case Client::RuiDad::CIdleState::AI_DASH_L:
		return new CDashState(OBJDIR::DIR_LEFT, false, false);
		break;
	case Client::RuiDad::CIdleState::AI_DASH_R:
		return new CDashState(OBJDIR::DIR_RIGHT, false, false);
		break;
	case Client::RuiDad::CIdleState::AI_JUMPATK:
		break;
	case Client::RuiDad::CIdleState::AI_JUMP:
		break;
	case Client::RuiDad::CIdleState::AI_RUSH:
		if (iRandom == 0)
			return new CSkill_RushState(TYPE_START);
		else
			return new CSkill_JumpDropState(TYPE_START);
		break;
	case Client::RuiDad::CIdleState::AI_HIT:
		break;
	case Client::RuiDad::CIdleState::AI_END:
		break;
	}


	return nullptr;
}

void CIdleState::Update_NearAttack()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 2:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 3:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 4:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 5:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 7:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 8:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 9:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 10:
		m_eState = AI_STATE::AI_DASH;
		break;
	}
}

void CIdleState::Update_NearMove()
{
	//std::random_device RandomDevice;
	//std::mt19937 gen(RandomDevice());
	//std::uniform_int_distribution<int> RandomPattern(1, 10);
	//int iRandom = RandomPattern(gen);

	//switch (iRandom)
	//{
	//case 1:
	//	m_eState = AI_STATE::AI_ATTACK;
	//	break;
	//case 2:
	//	m_eState = AI_STATE::AI_ATTACK;
	//	break;
	//case 3:
	//	m_eState = AI_STATE::AI_ATTACK;
	//	break;
	//case 4:
	//	m_eState = AI_STATE::AI_SKILL;
	//	break;
	//case 5:
	//	m_eState = AI_STATE::AI_SKILL;
	//	break;
	//case 6:
	//	m_eState = AI_STATE::AI_SKILL;
	//	break;
	//case 7:
	//	m_eState = AI_STATE::AI_JUMPATK;
	//	break;
	//case 8:
	//	m_eState = AI_STATE::AI_DASH;
	//	break;
	//case 9:
	//	m_eState = AI_STATE::AI_DASH;
	//	break;
	//case 10:
	//	m_eState = AI_STATE::AI_DASH;
	//	break;

	//}

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 2:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 3:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 4:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 7:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 8:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 9:
		m_eState = AI_STATE::AI_SKILL_0;
		break;
	case 10:
		m_eState = AI_STATE::AI_SKILL_0;
		break;

	}
}

void CIdleState::Update_NearGuard()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 2:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 3:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 4:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 7:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 8:
		m_eState = AI_STATE::AI_JUMPATK;
		break;
	case 9:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 10:
		m_eState = AI_STATE::AI_DASH;
		break;
	}
}

void CIdleState::Update_NearRush()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 2:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 3:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 4:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 5:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 7:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 8:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 9:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 10:
		m_eState = AI_STATE::AI_DASH;
		break;
	}
}

void CIdleState::Update_NearSkill()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);



	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 2:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 3:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 4:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 7:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 8:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 9:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 10:
		m_eState = AI_STATE::AI_DASH;
		break;
	}
}

_bool CIdleState::Far_CompareOriginPoint(CRuiDad * pRuiDad)
{
	_vector vTargetPosition = m_vOriginPosition;
	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

	if (fDistance <= 20.f)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CIdleState::Update_Far_InAttack()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_1;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_0;
		break;
	case 4:
		m_eState = AI_SKILL_2;
		break;
	case 5:
		m_eState = AI_SKILL_3;
		break;
	case 6:
		m_eState = AI_IDLE;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_DASH_B;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	}
}

void CIdleState::Update_Far_InMove()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);



	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_0;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_1;
		break;
	case 4:
		m_eState = AI_SKILL_2;
		break;
	case 5:
		m_eState = AI_SKILL_2;
		break;
	case 6:
		m_eState = AI_ATTACK;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_DASH_B;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	}
}

void CIdleState::Update_Far_InGuard()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 12);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_1;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_2;
		break;
	case 4:
		m_eState = AI_SKILL_2;
		break;
	case 5:
		m_eState = AI_SKILL_3;
		break;
	case 6:
		m_eState = AI_SKILL_3;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_RUSH;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_B;
		break;
	case 12:
		m_eState = AI_DASH_F;
		break;
	}
}

void CIdleState::Update_Far_InRush()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);



	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_0;
		break;
	case 2:
		m_eState = AI_IDLE;
		break;
	case 3:
		m_eState = AI_IDLE;
		break;
	case 4:
		m_eState = AI_GUARD;
		break;
	case 5:
		m_eState = AI_DASH_R;
		break;
	case 6:
		m_eState = AI_DASH_L;
		break;
	case 7:
		m_eState = AI_RUSH;
		break;
	case 8:
		m_eState = AI_DASH_B;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_ATTACK;
		break;
	}
}

void CIdleState::Update_Far_InSkill()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_1;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_1;
		break;
	case 4:
		m_eState = AI_SKILL_2;
		break;
	case 5:
		m_eState = AI_SKILL_2;
		break;
	case 6:
		m_eState = AI_SKILL_3;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_DASH_B;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	}
}

void CIdleState::Update_Far_OutAttack()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_1;
		break;
	case 2:
		m_eState = AI_SKILL_2;
		break;
	case 3:
		m_eState = AI_SKILL_2;
		break;
	case 4:
		m_eState = AI_SKILL_3;
		break;
	case 5:
		m_eState = AI_SKILL_3;
		break;
	case 6:
		m_eState = AI_FRONTMOVE;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_DASH_B;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	}
}

void CIdleState::Update_Far_OutMove()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 11);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_1;
		break;
	case 2:
		m_eState = AI_SKILL_2;
		break;
	case 3:
		m_eState = AI_SKILL_2;
		break;
	case 4:
		m_eState = AI_SKILL_3;
		break;
	case 5:
		m_eState = AI_SKILL_3;
		break;
	case 6:
		m_eState = AI_FRONTMOVE;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_DASH_B;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	}
}

void CIdleState::Update_Far_OutGuard()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 12);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_1;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_2;
		break;
	case 4:
		m_eState = AI_SKILL_2;
		break;
	case 5:
		m_eState = AI_SKILL_3;
		break;
	case 6:
		m_eState = AI_SKILL_3;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_RUSH;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	case 12:
		m_eState = AI_FRONTMOVE;
		break;
	}
}

void CIdleState::Update_Far_OutRush()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 12);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_GUARD;
		break;
	case 2:
		m_eState = AI_SKILL_0;
		break;
	case 3:
		m_eState = AI_SKILL_1;
		break;
	case 4:
		m_eState = AI_SKILL_2;
		break;
	case 5:
		m_eState = AI_SKILL_3;
		break;
	case 6:
		m_eState = AI_ATTACK;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_RUSH;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	case 12:
		m_eState = AI_FRONTMOVE;
		break;
	}
}

void CIdleState::Update_Far_OutSkill()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 12);
	int iRandom = RandomPattern(gen);

	// 1 발사체
	// 2 3방향
	// 3 가드

	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_0;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_2;
		break;
	case 4:
		m_eState = AI_SKILL_3;
		break;
	case 5:
		m_eState = AI_ATTACK;
		break;
	case 6:
		m_eState = AI_IDLE;
		break;
	case 7:
		m_eState = AI_IDLE;
		break;
	case 8:
		m_eState = AI_RUSH;
		break;
	case 9:
		m_eState = AI_DASH_L;
		break;
	case 10:
		m_eState = AI_DASH_R;
		break;
	case 11:
		m_eState = AI_DASH_F;
		break;
	case 12:
		m_eState = AI_FRONTMOVE;
		break;
	}
}

void CIdleState::Near_Skill_Setting(CRuiDad * pRuiDad)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);




	switch (iRandom)
	{
	case 1:
		m_eState = AI_SKILL_0;
		break;
	case 2:
		m_eState = AI_SKILL_1;
		break;
	case 3:
		m_eState = AI_SKILL_2;
		break;
	case 4:
		m_eState = AI_SKILL_3;
		break;
	}


}

void CIdleState::Dash_Setting(CRuiDad * pRuiDad)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_DASH_B;
		break;
	case 2:
		m_eState = AI_STATE::AI_DASH_F;
		break;
	case 3:
		DashDir_Calcul(pRuiDad);
		break;
	case 4:
		DashDir_Calcul(pRuiDad);
		break;
	}

	pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CIdleState::DashDir_Calcul(CRuiDad * pRuiDad)
{
	CCharacters* pTarget = pRuiDad->Get_BattleTarget();

	_vector vPlayerPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vUpVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vPlayerLook = XMVector3Normalize(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector vTargetLook = XMVector3Normalize(vPlayerPosition - vMyPosition);

	_vector vCross = XMVector3Cross(vPlayerLook, vTargetLook);
	_vector vDot = XMVector3Dot(vCross, vUpVector);


	if (XMVectorGetX(vDot) > 0.f)
	{
		m_eState = AI_STATE::AI_DASH_R;
	}
	else
	{
		m_eState = AI_STATE::AI_DASH_L;
	}
}

void CIdleState::Compare_OriginPoint(CRuiDad * pRuiDad)
{
}

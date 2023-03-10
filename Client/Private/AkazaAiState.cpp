#include "stdafx.h"
#include "AkazaAiState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

#include "AkazaAtk_1_State.h"

#include "AkazaAdvSkill_Common.h"
#include "AkazaAdvSkill_Move.h"

#include "AkazaDashState.h"
#include "AkazaMoveState.h"

#include "AkazaJumpAttackState.h"

#include "AkazaSkill_Destroy.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaGuardState.h"
#include "AkazaMoveJumpState.h"
#include "AkazaTargetRushState.h"
#include "AkazaJumpMoveAttackState.h"
#include "AkazaJumpState.h"


using namespace Akaza;


CAkazaAIState::CAkazaAIState()
{

}

CAkazaState * CAkazaAIState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Update_TargetState(pAkaza);


	switch (m_eRange)
	{
	case Client::Akaza::CAkazaAIState::RANGE_NEAR:
		pAkaza->Set_FarAI(false);
		Update_AI_Near(pAkaza);
		break;
	case Client::Akaza::CAkazaAIState::RANGE_FAR:
		pAkaza->Set_FarAI(true);
		Update_AI_Far(pAkaza);
		break;
	case Client::Akaza::CAkazaAIState::RANGE_OUT:
		pAkaza->Set_FarAI(true);
		Update_AI_Far(pAkaza);
		break;
	default:
		break;
	}





	if (m_fDashDelay >= 0.2f)
	{
		if (pAkaza->Get_IsDashPattern() == false)
		{
			Dash_CalCul(pAkaza);
			pAkaza->Set_DashPatten(true);

			if (m_eRange != RANGE_NEAR)
				return new CDashState(OBJDIR::DIR_STRAIGHT, false, false);

			if (m_eState == CAkazaAIState::AI_DASH_L)
				return new CDashState(OBJDIR::DIR_LEFT, false, false);
			else if (m_eState == CAkazaAIState::AI_DASH_R)
				return new CDashState(OBJDIR::DIR_RIGHT, false, false);

			
		}


		std::random_device RandomDevice;
		std::mt19937 gen(RandomDevice());
		std::uniform_int_distribution<int> RandomPattern(1, 10);
		int iRandom = RandomPattern(gen);

		if (iRandom == 5)
			pAkaza->Set_DashPatten(false);

		m_fDashDelay = 0.f;
	}




	if (m_fDelay <= 0.3f)
	{
		return nullptr;
	}
	else
	{
		m_fDelay = 0.f;
		return  Return_AIState(pAkaza);
	}
}

CAkazaState * CAkazaAIState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	m_fDelay += fTimeDelta;
	m_fDashDelay += fTimeDelta;

	return nullptr;
}

CAkazaState * CAkazaAIState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pAkaza->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pAkaza->Get_NavigationHeight().y + 0.1f)
	{
		pAkaza->Set_PlayerOriginPosY(fTimeDelta);
	}


	return nullptr;
}

void CAkazaAIState::Enter(CAkaza* pAkaza)
{
	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_IDLE);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_IDLE);
	pAkaza->Set_bGuard(false);
}

void CAkazaAIState::Exit(CAkaza* pAkaza)
{
	
}

void CAkazaAIState::Update_TargetState(CAkaza * pAkaza)
{
	CCharacters* pTarget = pAkaza->Get_BattleTarget();

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
	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance <= 30.f) { m_eRange = RANGE_IN; }
	else { m_eRange = RANGE_OUT; }


	if (m_eRange == RANGE_IN)
	{
		if (fDistance <= 15.f) { m_eRange = RANGE_NEAR; }
		else { m_eRange = RANGE_FAR; }
	}
}

void CAkazaAIState::Update_AI_Near(CAkaza * pAkaza)
{
	
	switch (m_eTargetState)
	{
	case Client::Akaza::CAkazaAIState::STATE_IDLE:
	case Client::Akaza::CAkazaAIState::STATE_MOVE:
	case Client::Akaza::CAkazaAIState::STATE_JUMP:
	case Client::Akaza::CAkazaAIState::STATE_DASH:
		Update_NearMove();
		break;
	case Client::Akaza::CAkazaAIState::STATE_ATTACK:
		Update_NearAttack();
		break;
	case Client::Akaza::CAkazaAIState::STATE_SKILL:
		Update_NearSkill();
		break;
	case Client::Akaza::CAkazaAIState::STATE_GUARD:
		Update_NearGuard();
		break;
	case Client::Akaza::CAkazaAIState::STATE_HIT:
		break;
	case Client::Akaza::CAkazaAIState::STATE_RUSH:
		Update_NearRush();
		break;
	case Client::Akaza::CAkazaAIState::STATE_END:
		break;
	default:
		break;
	}


}

void CAkazaAIState::Update_AI_Far(CAkaza * pAkaza)
{
	switch (m_eTargetState)
	{
	case Client::Akaza::CAkazaAIState::STATE_IDLE:
	case Client::Akaza::CAkazaAIState::STATE_MOVE:
	case Client::Akaza::CAkazaAIState::STATE_JUMP:
	case Client::Akaza::CAkazaAIState::STATE_DASH:
		Update_Far_InMove();
		break;
	case Client::Akaza::CAkazaAIState::STATE_ATTACK:
		Update_Far_InAttack();
		break;
	case Client::Akaza::CAkazaAIState::STATE_SKILL:
		Update_Far_InSkill();
		break;
	case Client::Akaza::CAkazaAIState::STATE_GUARD:
		Update_Far_InGuard();
		break;
	case Client::Akaza::CAkazaAIState::STATE_HIT:
		break;
	case Client::Akaza::CAkazaAIState::STATE_RUSH:
		Update_Far_InRush();
		break;
	case Client::Akaza::CAkazaAIState::STATE_END:
		break;
	default:
		break;
	}
}

void CAkazaAIState::Update_AI_Out(CAkaza * pAkaza)
{

}

void CAkazaAIState::Update_NearAttack()
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
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 7:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 8:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 9:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 10:
		m_eState = AI_STATE::AI_DASH;
		break;
	}



}

void CAkazaAIState::Update_NearMove()
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
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 7:
		m_eState = AI_STATE::AI_JUMPATK;
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


void CAkazaAIState::Update_NearGuard()
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

void CAkazaAIState::Update_NearRush()
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

void CAkazaAIState::Update_NearSkill()
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

void CAkazaAIState::Update_Far_InAttack()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 2:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 3:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 4:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 7:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 8:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 9:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 10:
		m_eState = AI_STATE::AI_RUSH;
		break;
	}


}

void CAkazaAIState::Update_Far_InMove()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 2:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 3:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 4:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 7:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 8:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 9:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 10:
		m_eState = AI_STATE::AI_RUSH;
		break;
	}
}

void CAkazaAIState::Update_Far_InGuard()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 2:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 3:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 4:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 7:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 8:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 9:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 10:
		m_eState = AI_STATE::AI_RUSH;
		break;
	}
}

void CAkazaAIState::Update_Far_InRush()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 2:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 3:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 4:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 7:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 8:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 9:
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 10:
		m_eState = AI_STATE::AI_RUSH;
		break;
	}
}

void CAkazaAIState::Update_Far_InSkill()
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 2:
		m_eState = AI_STATE::AI_IDLE;
		break;
	case 3:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 4:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 5:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 6:
		m_eState = AI_STATE::AI_DASH;
		break;
	case 7:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 8:
		m_eState = AI_STATE::AI_FRONTMOVE;
		break;
	case 9:
		m_eState = AI_STATE::AI_RUSH;
		break;
	case 10:
		m_eState = AI_STATE::AI_RUSH;
		break;
	}
}

CAkazaState* CAkazaAIState::Near_Skill_Setting(CAkaza * pAkaza)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 6);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		return new CSkill_PunchState(TYPE_START);
		break;
	case 2:
		return new CSkill_PunchState(TYPE_START);
		break;
	case 3:
		return new CSkill_DestoryState(TYPE_START);
		break;
	case 4:
		return new CSkill_ShootState(TYPE_START);
		break;
	case 5:
		return new CAdvSkill_CommmonState(TYPE_START);
		break;
	case 6:
		return new CAdvSkill_MoveState(TYPE_START);
		break;
	default:
		break;
	}

	return nullptr;
}

CAkazaState * CAkazaAIState::Return_AIState(CAkaza * pAkaza)
{

	if (m_eState == AI_STATE::AI_DASH)
	{
		Dash_CalCul(pAkaza);
	}
	
		

	switch (m_eState)
	{
	case Client::Akaza::CAkazaAIState::AI_IDLE:
		Enter(pAkaza);
		return nullptr;
		break;
	case Client::Akaza::CAkazaAIState::AI_BACKMOVE:
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		return new CMoveState(OBJDIR::DIR_STRAIGHT, TYPE_START);
		break;
	case Client::Akaza::CAkazaAIState::AI_FRONTMOVE:
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		return new CMoveState(OBJDIR::DIR_STRAIGHT, TYPE_START);
		break;
	case Client::Akaza::CAkazaAIState::AI_ATTACK:
		return new CAtk_1_State();
		break;
	case Client::Akaza::CAkazaAIState::AI_GUARD:
		return new CGuardState(TYPE_START);
		break;
	case Client::Akaza::CAkazaAIState::AI_SKILL:
		if (m_eRange == CAkazaAIState::RANGE_NEAR)
			return Near_Skill_Setting(pAkaza);
		else
			return new CSkill_ShootState(TYPE_START);
		break;
	case Client::Akaza::CAkazaAIState::AI_DASH:
		if (pAkaza->Get_IsFarAI() == true)
		{
			pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CDashState(OBJDIR::DIR_STRAIGHT, false, false);
		}
		else
		{
			if (m_eState == CAkazaAIState::AI_DASH_L)
				return new CDashState(OBJDIR::DIR_LEFT, false, false);
			else if (m_eState == CAkazaAIState::AI_DASH_R)
				return new CDashState(OBJDIR::DIR_RIGHT, false, false);
		}
		break;
	case Client::Akaza::CAkazaAIState::AI_JUMPATK:
		return new CJumpState(TYPE_START, 0.f,0.035f);
		break;
	case Client::Akaza::CAkazaAIState::AI_JUMP:
		break;
	case Client::Akaza::CAkazaAIState::AI_RUSH:
		return new CTargetRushState(TYPE_START);
		break;
	case Client::Akaza::CAkazaAIState::AI_HIT:
		break;
	case Client::Akaza::CAkazaAIState::AI_ADV_SKILL_0:
		break;
	case Client::Akaza::CAkazaAIState::AI_ADV_SKILL_1:
		break;
	case Client::Akaza::CAkazaAIState::AI_END:
		break;
	default:
		break;
	}





	return nullptr;
}

void CAkazaAIState::Dash_CalCul(CAkaza * pAkaza)
{
	CCharacters* pTarget = pAkaza->Get_BattleTarget();

	_vector vPlayerPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vUpVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vPlayerLook = XMVector3Normalize(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
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

	//월드 공간의 UP 방향 벡터를 U, 플레이어의 Forward 방향을 F, 플레이어에서 물체를 가르키는 방향을 A라고 했을 때, 
	//벡터 F와 벡터 A를 외적한다.이 외적한 벡터를 월드 벡터와 내적을 하였을 때, 양수이면 오른쪽 음수이면 왼쪽에 존재하게 된다.
}


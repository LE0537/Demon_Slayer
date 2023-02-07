#include "stdafx.h"
#include "..\Public\AiState.h"
#include "GameInstance.h"
#include <random>

// RuiState
#include "RuiMoveState.h"
#include "RuiIdleState.h"
#include "RuiAtk_1_State.h"
#include "RuiAtk_2_State.h"
#include "RuiAtk_3_State.h"
#include "RuiAtk_4_State.h"
#include "RuiSkill_Shoot.h"
#include "RuiGuardState.h"

#include "RuiJumpMoveAttackState.h"
#include "RuiAdvSkill_CommonState.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiSkill_Shoot.h"
#include "RuiSkill_ShootNet.h"
#include "RuiSkill_Sphere.h"
#include "RuiDashState.h"
#include "RuiTargetRushState.h"
#include "RuiJumpState.h"


using namespace Rui;

CAiState::CAiState()
{
}

CRuiState * CAiState::HandleInput(CRui * pRui)
{
	Update_TargetState(pRui);
	m_queueDash = pRui->Get_QueueCombo();

	if (m_queueDash.empty())
	{
		Compare_OriginPoint(pRui);
	}
	else
	{
		auto iter = m_queueDash.front();

		switch (iter)
		{
		case Client::Rui::CAiState::DASH_F:
			m_queueDash.pop();
			pRui->Set_QueueCombo(m_queueDash);
			pRui->Get_Transform()->Set_PlayerLookAt(m_vOriginPosition);
			return new CDashState(OBJDIR::DIR_STRAIGHT, false, false);
			break;
		case Client::Rui::CAiState::DASH_L:
			m_queueDash.pop();
			pRui->Set_QueueCombo(m_queueDash);
			pRui->Get_Transform()->Set_PlayerLookAt(m_vOriginPosition);
			return new CDashState(OBJDIR::DIR_LEFT, false, false);
			break;
		case Client::Rui::CAiState::DASH_R:
			m_queueDash.pop();
			pRui->Set_QueueCombo(m_queueDash);
			pRui->Get_Transform()->Set_PlayerLookAt(m_vOriginPosition);
			return new CDashState(OBJDIR::DIR_RIGHT, false, false);
			break;
		}
	}


	switch (m_eRange)
	{
	case Client::Rui::CAiState::RANGE_NEAR:
		Update_AI_Near(pRui);
		break;
	}


	
	
	if (pRui->Get_RuiHit() != true)
	{
		m_ePreState = m_eState;
		return Return_AIState(pRui);
	}
	else
	{
		m_ePreState = AI_HIT;
		return nullptr;
	}
}

CRuiState * CAiState::Tick(CRui * pRui, _float fTimeDelta)
{
	if (m_ePreState == AI_HIT)
		m_fDelay += fTimeDelta;

	//Update_TargetToAngle(pRui);
	


	return nullptr;
}

CRuiState * CAiState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CAiState::Enter(CRui * pRui)
{
	
	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_IDLE);
	pRui->Set_AnimIndex(CRui::ANIM_IDLE);
	pRui->Set_bGuard(false);

}

void CAiState::Exit(CRui * pRui)
{



}



void CAiState::Update_TargetState(CRui* pRui)
{
	CCharacters* pTarget = pRui->Get_BattleTarget();
	
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
	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance <= 30.f) {m_eRange = RANGE_IN;}
	else {m_eRange = RANGE_OUT;}


	if (m_eRange == RANGE_IN)
	{
		if (fDistance <= 15.f) { m_eRange = RANGE_NEAR; }
		else { m_eRange = RANGE_FAR; }
	}


}

void CAiState::Update_TargetToAngle(CRui * pRui)
{
	CCharacters* pTarget = pRui->Get_BattleTarget();
	_vector vTargetPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vRuiLook = XMVector3Normalize(pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector vTargetLook = XMVector3Normalize(vTargetPosition - vMyPosition);

	_vector DotRuiTarget = XMVector3AngleBetweenVectors(vRuiLook, vTargetLook);
	_float fAngle = XMConvertToDegrees(XMVectorGetX(DotRuiTarget));

	//pRui->Get_Transform()->Set_RotationY(fAngle);
	
	pRui->Get_Transform()->Set_PlayerLookAt(vTargetPosition);
}

void CAiState::Update_AI_Near(CRui* pRui)
{
	// TARGET STATE �˻�


	switch (m_eTargetState)
	{
	case Client::Rui::CAiState::STATE_IDLE:
	case Client::Rui::CAiState::STATE_MOVE:
	case Client::Rui::CAiState::STATE_JUMP:
	case Client::Rui::CAiState::STATE_DASH:
		Update_NearMove();
		break;
	case Client::Rui::CAiState::STATE_ATTACK:
		Update_NearAttack();
		break;
	case Client::Rui::CAiState::STATE_SKILL:
		Update_NearSkill();
		break;
	case Client::Rui::CAiState::STATE_GUARD:
		Update_NearGuard();
		break;
	case Client::Rui::CAiState::STATE_HIT:
		break;
	case Client::Rui::CAiState::STATE_RUSH:
		Update_NearRush();
		break;
	case Client::Rui::CAiState::STATE_END:
		break;
	default:
		break;
	}






}

void CAiState::Update_AI_Far(CRui* pRui)
{

}

CRuiState * CAiState::Return_AIState(CRui * pRui)
{
	// ���� AI ������Ʈ
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(0, 1);
	int iRandom = RandomPattern(gen);


	if (m_eState == AI_STATE::AI_DASH)
		Dash_Setting(pRui);
	else if (m_eState == AI_STATE::AI_SKILL)
		Near_Skill_Setting(pRui);


	switch (m_eState)
	{
	case Client::Rui::CAiState::AI_IDLE:
		return nullptr;
		break;
	case Client::Rui::CAiState::AI_BACKMOVE:
		return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		break;
	case Client::Rui::CAiState::AI_FRONTMOVE:
		return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		break;
	case Client::Rui::CAiState::AI_ATTACK:
		return new CAtk_1_State();
		break;
	case Client::Rui::CAiState::AI_JUMPATK:
		return new CJumpState(TYPE_START, 0.f, 0.f, true);
		break;
	case Client::Rui::CAiState::AI_GUARD:
		return new CGuardState(STATE_TYPE::TYPE_START);
		break;
	case Client::Rui::CAiState::AI_SKILL:
		break;
	case Client::Rui::CAiState::NEAR_AI_SKILL_1:
		return new CSkill_ShootState(TYPE_START);
		break;
	case Client::Rui::CAiState::NEAR_AI_SKILL_2:
		return new CSkill_ShootNetState(TYPE_START);
		break;
	case Client::Rui::CAiState::NEAR_AI_SKILL_3:
		return new CSkill_SphereState(TYPE_START);
		break;
	case Client::Rui::CAiState::AI_DASH_B:
		return new CDashState(OBJDIR::DIR_BACK, false, false);
		break;
	case Client::Rui::CAiState::AI_DASH_F:
		return new CDashState(OBJDIR::DIR_STRAIGHT, false, false);
		break;
	case Client::Rui::CAiState::AI_DASH_L:
		if(iRandom ==0)
			return new CDashState(OBJDIR::DIR_LEFT, false, false);
		else
			return new CDashState(OBJDIR::DIR_LEFT_DASH, false, false);
		break;
	case Client::Rui::CAiState::AI_DASH_R:
		if (iRandom == 0)
			return new CDashState(OBJDIR::DIR_RIGHT, false, false);
		else
			return new CDashState(OBJDIR::DIR_RIGHT_DASH, false, false);
		break;
	case Client::Rui::CAiState::AI_RUSH:
		return new CTargetRushState(TYPE_START);
		break;
	case Client::Rui::CAiState::AI_HIT:
		break;
	case Client::Rui::CAiState::AI_JUMP:
		break;
	case Client::Rui::CAiState::AI_END:
		break;
	default:
		break;
	}

	return nullptr;
}

void CAiState::Update_NearAttack()
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
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 6:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 7:
		m_eState = AI_STATE::AI_SKILL;
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

void CAiState::Update_NearMove()
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

void CAiState::Update_NearGuard()
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

void CAiState::Update_NearRush()
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

void CAiState::Update_NearSkill()
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

void CAiState::Near_Skill_Setting(CRui * pRui)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 5);
	int iRandom = RandomPattern(gen);

	// 1 �߻�ü
	// 2 3����
	// 3 ����

	switch (iRandom)
	{
	case 1:
		m_eState = NEAR_AI_SKILL_1;
		break;
	case 2:
		m_eState = NEAR_AI_SKILL_1;
		break;
	case 3:
		m_eState = NEAR_AI_SKILL_2;
		break;
	case 4:
		m_eState = NEAR_AI_SKILL_2;
		break;
	case 5:
		m_eState = NEAR_AI_SKILL_3;
		break;
	}



}

void CAiState::Dash_Setting(CRui* pRui)
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
		DashDir_Calcul(pRui);
		break;
	case 4:
		DashDir_Calcul(pRui);
		break;
	}

	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CAiState::DashDir_Calcul(CRui * pRui)
{
	CCharacters* pTarget = pRui->Get_BattleTarget();

	_vector vPlayerPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vUpVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vPlayerLook = XMVector3Normalize(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	_vector vTargetLook = XMVector3Normalize(vPlayerPosition - vMyPosition);

	_vector vCross = XMVector3Cross(vPlayerLook, vTargetLook);
	_vector vDot = XMVector3Dot(vCross, vUpVector);


	if (XMVectorGetX(vDot) > 0.f)
	{
		m_eState = AI_STATE::AI_DASH_L;
	}
	else
	{
		m_eState = AI_STATE::AI_DASH_R;
	}

	//���� ������ UP ���� ���͸� U, �÷��̾��� Forward ������ F, �÷��̾�� ��ü�� ����Ű�� ������ A��� ���� ��, 
	//���� F�� ���� A�� �����Ѵ�.�� ������ ���͸� ���� ���Ϳ� ������ �Ͽ��� ��, ����̸� ������ �����̸� ���ʿ� �����ϰ� �ȴ�.

}

void CAiState::Compare_OriginPoint(CRui * pRui)
{
	
	_vector vTargetPosition = m_vOriginPosition;
	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

	printf_s("dist : %f \n", fDistance);

	if (fDistance >= 42.f)
	{


		_vector vTargetPosition = m_vOriginPosition;
		_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

		_vector vUpVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vPlayerLook = XMVector3Normalize(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
		_vector vTargetLook = XMVector3Normalize(vTargetPosition - vMyPosition);

		_vector vCross = XMVector3Cross(vPlayerLook, vTargetLook);
		_vector vDot = XMVector3Dot(vCross, vUpVector);



		if (XMVectorGetX(vDot) > 0.f)
		{
			// ��� ������
			m_queueDash.push(DASH_R);
			//m_queueDash.push(DASH_R);
			m_queueDash.push(DASH_F);
			//m_queueDash.push(DASH_F);
			pRui->Set_QueueCombo(m_queueDash);
		}
		else
		{
			//m_queueDash.push(DASH_L);
			m_queueDash.push(DASH_L);
			//m_queueDash.push(DASH_F);
			m_queueDash.push(DASH_F);
			pRui->Set_QueueCombo(m_queueDash);
			// ���� ����	
		}
	}

}





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


using namespace Rui;

CAiState::CAiState()
{
}

CRuiState * CAiState::HandleInput(CRui * pRui)
{
	Update_TargetState(pRui);

	switch (m_eRange)
	{
	case Client::Rui::CAiState::RANGE_NEAR:
		Update_AI_Near(pRui);
		break;
	}

	
	if (pRui->Get_RuiHit() == true)
		return nullptr;
	else
		return Return_AIState(pRui);
}

CRuiState * CAiState::Tick(CRui * pRui, _float fTimeDelta)
{
	
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
		if (fDistance <= 12.f) { m_eRange = RANGE_NEAR; }
		else { m_eRange = RANGE_FAR; }
	}
	printf_s("dist : %f \n", fDistance);

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
	// TARGET STATE 검사


	switch (m_eTargetState)
	{
	case Client::Rui::CAiState::STATE_IDLE:
	case Client::Rui::CAiState::STATE_MOVE:
	case Client::Rui::CAiState::STATE_JUMP:
		break;
	case Client::Rui::CAiState::STATE_ATTACK:
		Update_NearAttack();
		break;
	case Client::Rui::CAiState::STATE_SKILL:
		break;
	case Client::Rui::CAiState::STATE_GUARD:
		break;
	case Client::Rui::CAiState::STATE_HIT:
		break;
	case Client::Rui::CAiState::STATE_RUSH:
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
	// 최종 AI 스테이트

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
	case Client::Rui::CAiState::AI_GUARD:
		return new CGuardState(STATE_TYPE::TYPE_START);
		break;
	case Client::Rui::CAiState::AI_SKILL:
		break;
	case Client::Rui::CAiState::AI_DASH:
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
		m_eState = AI_STATE::AI_GUARD;
		break;
	case 4:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 5:
		m_eState = AI_STATE::AI_ATTACK;
		break;
	case 6:
		m_eState = AI_STATE::AI_BACKMOVE;
		break;
	case 7:
		m_eState = AI_STATE::AI_BACKMOVE;
		break;
	case 8:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 9:
		m_eState = AI_STATE::AI_SKILL;
		break;
	case 10:
		m_eState = AI_STATE::AI_DASH;
		break;
	}

}



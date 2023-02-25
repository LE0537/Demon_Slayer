#include "stdafx.h"
#include "EnmuIdlestate.h"
#include "GameInstance.h"
#include "Effect_Manager.h"




#include "EnmuAttack_1.h"
#include "EnmuMoveState.h"
#include "EnmuTargetRushState.h"
#include "EnmuGuardState.h"
#include "EnmuDashState.h"
#include "Skill1_EnmuState.h"
#include "Skill2_EnmuState.h"
#include "Skill3_EnmuState.h"

using namespace Enmu;

CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
	
}

CEnmuState * CIdleState::HandleInput(CEnmu* pEnmu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	//Update_TargetState(pEnmu);

	//switch (m_eRange)
	//{
	//case Client::Enmu::CIdleState::RANGE_IN:
	//	Update_AI_Near(pEnmu);
	//	break;
	//case Client::Enmu::CIdleState::RANGE_OUT:
	//	Update_AI_Out(pEnmu);
	//	break;
	//default:
	//	break;
	//}


	if (pGameInstance->Get_Instance()->Key_Down(DIK_1))
		return new CEnmuAttack1(TYPE_START);

	if (pGameInstance->Get_Instance()->Key_Down(DIK_2))
		return new CSkill1_EnmuState(TYPE_START);

	if (pGameInstance->Get_Instance()->Key_Down(DIK_3))
		return new CSkill2_EnmuState(TYPE_START);

	if (pGameInstance->Get_Instance()->Key_Down(DIK_4))
		return new CSkill3_EnmuState(TYPE_START);





	return Return_AIState(pEnmu);
}

CEnmuState * CIdleState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{


	return nullptr;
}

CEnmuState * CIdleState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pEnmu->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pEnmu->Get_NavigationHeight().y + 0.1f)
	{
		pEnmu->Set_PlayerOriginPosY(fTimeDelta);
	}

	if (pEnmu->Get_PlayerInfo().iGuard < pEnmu->Get_PlayerInfo().iMaxGuard)
	{
		pEnmu->Set_GuardHp(1);
		if (pEnmu->Get_PlayerInfo().iGuard > pEnmu->Get_PlayerInfo().iMaxGuard)
			pEnmu->Set_ResetGuardHp();
	}

	return nullptr;
}

void CIdleState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_IDLE;


	pEnmu->Set_GodMode(false);
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_IDLE);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_IDLE);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_IDLE);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_IDLE, 0.05f);
	

}

void CIdleState::Exit(CEnmu* pEnmu)
{
}

void CIdleState::Update_TargetState(CEnmu * pEnmu)
{
	CCharacters* pTarget = pEnmu->Get_BattleTarget();

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
	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance <= 20.f) { m_eRange = RANGE_IN; }
	else { m_eRange = RANGE_OUT; }




}

void CIdleState::Update_AI_Near(CEnmu * pEnmu)
{

	switch (m_eTargetState)
	{
	case Client::Enmu::CIdleState::STATE_IDLE:
	case Client::Enmu::CIdleState::STATE_MOVE:
	case Client::Enmu::CIdleState::STATE_JUMP:
	case Client::Enmu::CIdleState::STATE_DASH:
		Update_Near_Move();
		break;
	case Client::Enmu::CIdleState::STATE_ATTACK:
		Update_Near_Attack();
		break;
	case Client::Enmu::CIdleState::STATE_SKILL:
		Update_Near_Skill();
		break;
	case Client::Enmu::CIdleState::STATE_GUARD:
		Update_Near_Guard();
		break;
	case Client::Enmu::CIdleState::STATE_HIT:
		break;
	case Client::Enmu::CIdleState::STATE_RUSH:
		Update_Near_Rush();
		break;
	case Client::Enmu::CIdleState::STATE_END:
		break;
	default:
		break;
	}



}

void CIdleState::Update_AI_Out(CEnmu * pEnmu)
{





}

void CIdleState::Update_Near_Attack()
{
}

void CIdleState::Update_Near_Move()
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
		m_eState = AI_STATE::AI_IDLE;
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

void CIdleState::Update_Near_Guard()
{


}

void CIdleState::Update_Near_Rush()
{

}

void CIdleState::Update_Near_Skill()
{
}

CEnmuState* CIdleState::Near_Skill_Setting(CEnmu * pEnmu)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		return new CSkill1_EnmuState(TYPE_START);
		break;
	case 2:
		return new CSkill2_EnmuState(TYPE_START);
		break;
	case 3:
		return new CSkill3_EnmuState(TYPE_START);
		break;
	default:
		break;
	}


	return nullptr;
}



CEnmuState * CIdleState::Return_AIState(CEnmu * pEnmu)
{
	switch (m_eState)
	{
	case Client::Enmu::CIdleState::AI_IDLE:
		Enter(pEnmu);
		return nullptr;
		break;
	case Client::Enmu::CIdleState::AI_BACKMOVE:
		break;
	case Client::Enmu::CIdleState::AI_FRONTMOVE:
		break;
	case Client::Enmu::CIdleState::AI_ATTACK:
		return new CEnmuAttack1(TYPE_START);
		break;
	case Client::Enmu::CIdleState::AI_GUARD:
		break;
	case Client::Enmu::CIdleState::AI_SKILL:
		return Near_Skill_Setting(pEnmu);
		break;
	case Client::Enmu::CIdleState::AI_DASH:
		break;
	case Client::Enmu::CIdleState::AI_DASH_F:
		break;
	case Client::Enmu::CIdleState::AI_DASH_B:
		break;
	case Client::Enmu::CIdleState::AI_DASH_L:
		break;
	case Client::Enmu::CIdleState::AI_DASH_R:
		break;
	case Client::Enmu::CIdleState::AI_JUMPATK:
		break;
	case Client::Enmu::CIdleState::AI_JUMP:
		break;
	case Client::Enmu::CIdleState::AI_RUSH:
		break;
	case Client::Enmu::CIdleState::AI_HIT:
		break;
	case Client::Enmu::CIdleState::AI_SKILL_0:
		break;
	case Client::Enmu::CIdleState::AI_SKILL_1:
		break;
	case Client::Enmu::CIdleState::AI_SKILL_2:
		break;
	case Client::Enmu::CIdleState::AI_SKILL_3:
		break;
	case Client::Enmu::CIdleState::AI_END:
		break;
	default:
		break;
	}



	return nullptr;
}


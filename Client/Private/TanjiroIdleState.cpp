#include "stdafx.h"
#include "TanjiroIdlestate.h"
#include "TanjiroMoveState.h"
#include "TanjiroJumpstate.h"
#include "GameInstance.h"

#include "TanjiroAtk_1_State.h"
#include "TanjiroGuardState.h"
#include "TanjiroGuardHitState.h"
using namespace Tanjiro;



CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
}

CTanjiroState * CIdleState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_W)) // ��
	{
		if (pGameInstance->Key_Pressing(DIK_A)) // ��
			return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
		else
			return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
	}

	else if (pGameInstance->Key_Pressing(DIK_D)) // ��
	{
		if (pGameInstance->Key_Pressing(DIK_A)) // ��
			return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
			return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
		else
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
	}


	else if (pGameInstance->Key_Pressing(DIK_A)) // ��
		return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
	else if (pGameInstance->Key_Pressing(DIK_S)) // ��
		return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
	else if (pGameInstance->Key_Down(DIK_SPACE)) // ����
	{
		_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fPositionY = XMVectorGetY(vPosition);
		return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
	}
	//else if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
	//	return new CAtk_1_State();
	else if (pGameInstance->Key_Down(DIK_J))
		return new CAtk_1_State();

	if (pGameInstance->Key_Pressing(DIK_O))
		return new CGuardState(STATE_TYPE::TYPE_START);
	else if (pGameInstance->Key_Pressing(DIK_P))
		return new CGuardHitState(STATE_TYPE::TYPE_START);
	else if (pGameInstance->Key_Down(DIK_I))
		return new CGuardHitState(STATE_TYPE::TYPE_LOOP);

	return nullptr;
}

CTanjiroState * CIdleState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{



	return nullptr;
}

CTanjiroState * CIdleState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fTest = 0.f;
	fTest += fTimeDelta;


	if (ePreState == CTanjiroState::STATE_JUMP)
	{
		pTanjiro->Get_Model()->Play_Animation2(fTimeDelta);
	}
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CIdleState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_IDLE);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);

}

void CIdleState::Exit(CTanjiro * pTanjiro)
{
}


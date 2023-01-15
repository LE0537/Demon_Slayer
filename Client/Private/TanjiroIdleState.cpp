#include "stdafx.h"
#include "TanjiroIdlestate.h"
#include "TanjiroMoveState.h"
#include "GameInstance.h"

using namespace Tanjiro;



CIdleState::CIdleState()
{
}

CTanjiroState * CIdleState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_W)) // 菊
	{
		if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
		else
			return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
	}
		
	else if (pGameInstance->Key_Pressing(DIK_D)) // 第
	{
		if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
			return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
		else
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
	}


	else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
		return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
	else if (pGameInstance->Key_Pressing(DIK_S)) // 快
		return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);





	return nullptr;
}

CTanjiroState * CIdleState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	//pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	

	return nullptr;
}

CTanjiroState * CIdleState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
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


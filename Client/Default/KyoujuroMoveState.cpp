#include "stdafx.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroIdleState.h"
#include "Kyoujuro.h"
#include "GameInstance.h"

using namespace Kyoujuro;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CKyoujuroState * CMoveState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
	{
		if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
		else
			return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
	}

	else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
	{
		if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
			return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
		else
			return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
	}


	else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
		return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
	else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
		return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
	else
		return new CIdleState();



	return nullptr;
}

CKyoujuroState * CMoveState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			m_eStateType = CKyoujuroState::TYPE_LOOP;
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	return nullptr;


}

CKyoujuroState * CMoveState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	Move(pKyoujuro, fTimeDelta);
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CMoveState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_MOVE_START);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_START);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_MOVE_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_END);
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CKyoujuro * pKyoujuro)
{
}

void CMoveState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pKyoujuro->Get_Transform()->Set_RotationY(0.f);
		break;
	case Client::DIR_LEFT:
		pKyoujuro->Get_Transform()->Set_RotationY(270.f);
		break;
	case Client::DIR_RIGHT:
		pKyoujuro->Get_Transform()->Set_RotationY(90.f);
		break;
	case Client::DIR_BACK:
		pKyoujuro->Get_Transform()->Set_RotationY(180.f);
		break;
	case Client::DIR_LF:
		pKyoujuro->Get_Transform()->Set_RotationY(305.f);
		break;
	case Client::DIR_RF:
		pKyoujuro->Get_Transform()->Set_RotationY(45.f);
		break;
	case Client::DIR_LB:
		pKyoujuro->Get_Transform()->Set_RotationY(225.f);
		break;
	case Client::DIR_RB:
		pKyoujuro->Get_Transform()->Set_RotationY(135.f);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP)
		pKyoujuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
}

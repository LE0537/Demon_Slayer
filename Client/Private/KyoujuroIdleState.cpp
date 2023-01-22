#include "stdafx.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroJumpState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_1_State.h"
#include "KyoujuroGuardState.h"
#include "KyoujuroGuardHitState.h"

using namespace Kyoujuro;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CKyoujuroState * CIdleState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pKyoujuro->Set_bGuard(false);

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_W)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // ��
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_D)) // ��
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // ��
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // ��
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_SPACE))
		{
			_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_LOOP, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_START);
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_UP)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // �� 
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_RCONTROL))
		{
			_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_LOOP, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_C))
			return new CGuardState(STATE_TYPE::TYPE_START);
		break;
	default:
		break;
	}



	return nullptr;
}

CKyoujuroState * CIdleState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{



	return nullptr;
}

CKyoujuroState * CIdleState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	if (m_ePreState == CKyoujuroState::STATE_JUMP)
	{
		pKyoujuro->Get_Model()->Play_Animation2(fTimeDelta);
	}
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CIdleState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_IDLE);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_IDLE);
}

void CIdleState::Exit(CKyoujuro * pKyoujuro)
{
}

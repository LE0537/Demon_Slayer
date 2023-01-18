#include "stdafx.h"
#include "KyoujuroMoveJumpState.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroJumpState.h"

#include "GameInstance.h"

using namespace Kyoujuro;


CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CKyoujuroState * CMoveJumpState::HandleInput(CKyoujuro * pKyoujuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	if (m_eStateType != CKyoujuroState::TYPE_DEFAULT)
	{
		if (pGameInstance->Key_Pressing(DIK_UP)) // 앞
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LF;
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_RF;
			}
			else
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_STRAIGHT;
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 뒤
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LB;
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우 
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_RB;
			}
			else
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_BACK;
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
		{
			m_bMove = true;
			m_eNextDir = OBJDIR::DIR_LEFT;
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
		{
			m_bMove = true;
			m_eNextDir = OBJDIR::DIR_RIGHT;
		}
		else
		{
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}
	}
	else
	{
		m_bMove = false;
		m_eNextDir = OBJDIR::DIR_STOP;
	}


	

	return nullptr;
}

CKyoujuroState * CMoveJumpState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_START);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_LOOP_START);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_LOOP_END);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_END);


	if (m_eStateType == CKyoujuroState::TYPE_CHANGE)
	{
		//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_START);
	}
	


	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			printf_s("Start Jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CMoveJumpState(m_eNextDir, STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			printf_s("Loop Jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			return new CMoveJumpState(m_eNextDir, STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_END:
			printf_s("End jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			printf_s("Default Jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}


	return nullptr;
}

CKyoujuroState * CMoveJumpState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);
	else if (m_eStateType == TYPE_LOOP)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else if (m_eStateType == TYPE_DEFAULT)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.05f;


	if (m_eStateType != TYPE_DEFAULT)
		Jump(pKyoujuro, fTimeDelta + m_fJumpTime);

	if(m_bMove == true)
		Move(pKyoujuro, fTimeDelta);

	return nullptr;
}

void CMoveJumpState::Enter(CKyoujuro * pKyoujuro)
{

	m_eStateId = CKyoujuroState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_START, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_START);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_LOOP_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_LOOP_START, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_LOOP_END, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_MOVE_START);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_START);
		break;
	}


}

void CMoveJumpState::Exit(CKyoujuro * pKyoujuro)
{
	pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
}

void CMoveJumpState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
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

	if (m_eDirection != DIR_STOP && m_bMove == true)
		pKyoujuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
}

CKyoujuroState*  CMoveJumpState::Jump(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	

	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		if (m_eNextDir == OBJDIR::DIR_STOP)
		{
			m_eStateType = CKyoujuroState::TYPE_DEFAULT;
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_END);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = CKyoujuroState::TYPE_CHANGE;
		}
		
	} 

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}


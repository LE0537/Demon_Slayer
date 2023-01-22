#include "stdafx.h"
#include "TanjiroMoveJumpState.h"

#include "TanjiroMoveState.h"
#include "TanjiroIdlestate.h"
#include "GameInstance.h"


using namespace Tanjiro;

CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CTanjiroState * CMoveJumpState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (m_eStateType != STATE_TYPE::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Pressing(DIK_W)) // ¾Õ
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LF;
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì
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

			else if (pGameInstance->Key_Pressing(DIK_S)) // µÚ
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LB;
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì 
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


			else if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LEFT;
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì
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
		break;
	case 2:
		if (m_eStateType != STATE_TYPE::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Pressing(DIK_UP)) // ¾Õ
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LF;
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì
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

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // µÚ
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LB;
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì 
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


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LEFT;
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì
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
		break;
	default:
		break;
	}



	return nullptr;
}

CTanjiroState * CMoveJumpState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÃÑ ½Ã°£
	_float fCurrentTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÇöÀç ½Ã°£


	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_END);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);



	if (m_eStateType == CTanjiroState::TYPE_CHANGE)
	{
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_LOOP);
	}


	else if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			printf_s("Start Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

			return new CMoveJumpState(m_eDirection ,STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			printf_s("Loop Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

			return new CMoveJumpState(m_eDirection, STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_END:
			printf_s("End jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			printf_s("Default Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			if(m_bMove == false)
				return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	

	return nullptr;
}

CTanjiroState * CMoveJumpState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 3.f);
	else if (m_eStateType == TYPE_LOOP)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	else if (m_eStateType == TYPE_DEFAULT)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.05f;

	if (m_eStateType != TYPE_DEFAULT && m_eStateType != TYPE_CHANGE)
		Jump(pTanjiro, fTimeDelta + m_fJumpTime);

	if (m_bMove == true)
		Move(pTanjiro, fTimeDelta);

	return nullptr;
}

void CMoveJumpState::Enter(CTanjiro * pTanjiro)
{

	m_eStateId = CTanjiroState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_START);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_END, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
		break;
	case Client::CTanjiroState::TYPE_CHANGE:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_MOVE_LOOP);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);

		break;
	}


}

void CMoveJumpState::Exit(CTanjiro * pTanjiro)
{
}

void CMoveJumpState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fCamAngle = pTanjiro->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pTanjiro->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pTanjiro->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pTanjiro->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pTanjiro->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pTanjiro->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pTanjiro->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pTanjiro->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pTanjiro->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP && m_bMove == true)
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
}

CTanjiroState*  CMoveJumpState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


		if (m_bMove == false)
		{
			m_eStateType = CTanjiroState::TYPE_DEFAULT;
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = CTanjiroState::TYPE_CHANGE;
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_MOVE_LOOP);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);
		}
	}

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}




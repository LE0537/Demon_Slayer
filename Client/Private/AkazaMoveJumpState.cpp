#include "stdafx.h"
#include "AkazaMoveJumpState.h"
#include "AkazaMoveState.h"
#include "AkazaIdleState.h"
#include "AkazaJumpState.h"
#include "Layer.h"
#include "GameInstance.h"

using namespace Akaza;


CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CAkazaState * CMoveJumpState::HandleInput(CAkaza* pAkaza)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (m_eStateType != CAkazaState::TYPE_END)
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
		if (m_eStateType != CAkazaState::TYPE_END)
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
	}

	return nullptr;
}

CAkazaState * CMoveJumpState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÃÑ ½Ã°£
	_float fCurrentTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÇöÀç ½Ã°£

	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_START);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_LOOP);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_JUMP_END);

	if (m_eStateType == CAkazaState::TYPE_CHANGE)
	{
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_START);
	}
	

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}


	return nullptr;
}

CAkazaState * CMoveJumpState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	m_fJumpTime += 0.05f;

	if(m_eStateType != STATE_TYPE::TYPE_END)
		Jump(pAkaza, fTimeDelta + m_fJumpTime);

	if(m_bMove == true)
		Move(pAkaza, fTimeDelta);

	pAkaza->Get_Model()->Play_Animation(fTimeDelta,true);

	return nullptr;
}

void CMoveJumpState::Enter(CAkaza* pAkaza)
{

	m_eStateId = CAkazaState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_START);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_START, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_START);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_LOOP);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_LOOP, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_LOOP);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_END, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
		break;
	}


}

void CMoveJumpState::Exit(CAkaza* pAkaza)
{
}

void CMoveJumpState::Move(CAkaza* CAkaza, _float fTimeDelta)
{
	_float fCamAngle = CAkaza->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		CAkaza->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		CAkaza->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		CAkaza->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		CAkaza->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		CAkaza->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		CAkaza->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		CAkaza->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		CAkaza->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP && m_bMove == true)
	{
		CAkaza->Get_Transform()->Go_Straight(fTimeDelta, CAkaza->Get_NavigationCom());
		CCharacters* m_pTarget = CAkaza->Get_BattleTarget();
		CCollider*	pMyCollider = CAkaza->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = CAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = CAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(CAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = CAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (CAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
				CAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				CAkaza->Get_Transform()->Go_Backward(fTimeDelta / 2.f, CAkaza->Get_NavigationCom());

		}
	}
}
CAkazaState*  CMoveJumpState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	

	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


		if (m_bMove == false)
		{
			m_eStateType = TYPE_END;
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_JUMP_END);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_JUMP_END, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = STATE_TYPE::TYPE_CHANGE;
		}
	} 

	pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}


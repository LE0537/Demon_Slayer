#include "stdafx.h"
#include "ShinobuMoveState.h"
#include "ShinobuIdlestate.h"
#include "ShinobuMoveJumpState.h"
#include "GameInstance.h"
#include "ShinobuAtk_1_State.h"
#include "Characters.h"
#include "Layer.h"
#include "ShinobuGuardState.h"
#include "ShinobuDashState.h"
#include "ShinobuChangeState.h"
#include "ShinobuSkill_Move.h"
#include "ShinobuSkill_Upper.h"
#include "Effect_Manager.h"
using namespace Shinobu;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CShinobuState * CMoveState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pShinobu->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_O) && pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{

				if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
				{
					//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_WINDMILL);
					pShinobu->Set_SkillBar(-200);
					return new CSkill_UpperState(STATE_TYPE::TYPE_START);
				}

			}
			else
			{

				if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
				{
					pShinobu->Set_SkillBar(-200);
					return new CSkill_MoveState(STATE_TYPE::TYPE_START);
				}

			}
		}


		if (pGameInstance->Key_Pressing(DIK_W)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LF);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LF, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RF);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RF, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_STRAIGHT);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, CShinobuState::TYPE_START, fPositionY, 0.f);
				}

				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_LOOP);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LB);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_LB, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RB);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RB, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_BACK, CShinobuState::TYPE_START, fPositionY, 0.f);
				}

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_BACK);

				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_LOOP);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
		{

			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_LEFT);

			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LEFT, CShinobuState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_RIGHT);

			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RIGHT, CShinobuState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CMoveState(OBJDIR::DIR_STOP, TYPE_END);
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_C) && pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{

				if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
				{
					//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM);
					pShinobu->Set_SkillBar(-200);
					return new CSkill_UpperState(STATE_TYPE::TYPE_START);
				}

			}
			else
			{

				if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
				{
					pShinobu->Set_SkillBar(-200);
					return new CSkill_MoveState(STATE_TYPE::TYPE_START);
				}

			}
		}

		if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{

				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LF);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_LF, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			{

				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RF);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RF, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_STRAIGHT);


				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, CShinobuState::TYPE_START, fPositionY, 0.f);
				}

				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_LOOP);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{

				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LB);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_LB, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
			{

				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RB);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RB, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_BACK);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_BACK, CShinobuState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_LOOP);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_LEFT);

			if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LEFT, CShinobuState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_RIGHT);

			if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RIGHT, CShinobuState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CMoveState(OBJDIR::DIR_STOP, TYPE_END);
		break;
	default:
		break;
	}




	return nullptr;
}

CShinobuState * CMoveState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CMoveState(m_eDirection, TYPE_START);
			break;
			//case Client::CTanjiroState::TYPE_LOOP:
			//	m_eStateType = CTanjiroState::TYPE_LOOP;
			//	break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CShinobuState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	return nullptr;
}

CShinobuState * CMoveState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	Move(pShinobu, fTimeDelta);
	pShinobu->Get_Model()->Play_Animation(fTimeDelta * 0.85f);
	if (pShinobu->Get_PlayerInfo().bChange)
	{
		return new CIdleState();
	}
	if (pShinobu->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}
	if (pShinobu->Get_PlayerInfo().iGuard < pShinobu->Get_PlayerInfo().iMaxGuard)
	{
		pShinobu->Set_GuardHp(1);
		if (pShinobu->Get_PlayerInfo().iGuard > pShinobu->Get_PlayerInfo().iMaxGuard)
			pShinobu->Set_ResetGuardHp();
	}
	return nullptr;
}

void CMoveState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_MOVE_START);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_MOVE_START);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), true);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_MOVE_START, 0.01f);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_MOVE_LOOP);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_MOVE_LOOP);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), true);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_MOVE_LOOP, 0.01f);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_MOVE_END);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_MOVE_END);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_MOVE_END, 0.01f);
		break;
	case Client::CShinobuState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CShinobu* pShinobu)
{
}

void CMoveState::Move(CShinobu* pShinobu, _float fTimeDelta)
{
	_float fCamAngle = pShinobu->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pShinobu->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pShinobu->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pShinobu->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pShinobu->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pShinobu->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pShinobu->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pShinobu->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pShinobu->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_eDirection != DIR_STOP)
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta, pShinobu->Get_NavigationCom());
	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	CCollider*	pMyCollider = pShinobu->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{

		_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pShinobu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pShinobu->Get_NavigationCom());

	}

	RELEASE_INSTANCE(CGameInstance);
}



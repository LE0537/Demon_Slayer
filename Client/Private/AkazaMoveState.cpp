#include "stdafx.h"
#include "AkazaMoveState.h"
#include "AkazaIdleState.h"
#include "Akaza.h"
#include "GameInstance.h"
#include "Characters.h"
#include "Layer.h"
#include "AkazaDashState.h"
#include "AkazaAtk_1_State.h"
#include "AkazaMoveJumpState.h"
#include "AkazaGuardState.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Destroy.h"

using namespace Akaza;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CAkazaState * CMoveState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					pAkaza->Set_SkillBar(-200);
					return new CSkill_DestoryState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					pAkaza->Set_SkillBar(-200);
					return new CSkill_PunchState(STATE_TYPE::TYPE_START);
				}
			}
		}

		if (pGameInstance->Key_Pressing(DIK_W)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LF);

				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}

				else
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RF);

				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_STRAIGHT);
				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LB);
				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RB);
				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_BACK);
				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}

		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_LEFT);
			else if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_RIGHT);

			else if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else
			return new CIdleState();
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_C))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					pAkaza->Set_SkillBar(-200);
					return new CSkill_DestoryState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					pAkaza->Set_SkillBar(-200);
					return new CSkill_PunchState(STATE_TYPE::TYPE_START);
				}
			}
		}


		if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LF);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RF);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_STRAIGHT);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LB);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RB);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_BACK);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_LEFT);
			else if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_RIGHT);
			else if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else
			return new CIdleState();

		break;
	}



	return nullptr;
}

CAkazaState * CMoveState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			m_eStateType = CAkazaState::TYPE_LOOP;
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CAkazaState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;


}

CAkazaState * CMoveState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	Move(pAkaza, fTimeDelta);
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	if (pAkaza->Get_PlayerInfo().bSub)
	{
		return new CIdleState();
	}

	return nullptr;
}

void CMoveState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_MOVE_START);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_MOVE_START);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_MOVE_END);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_MOVE_END);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CAkaza* pAkaza)
{
}

void CMoveState::Move(CAkaza* pAkaza, _float fTimeDelta)
{
	_float fCamAngle = pAkaza->Get_CamAngle();
	
	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pAkaza->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pAkaza->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pAkaza->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pAkaza->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pAkaza->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pAkaza->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pAkaza->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pAkaza->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP)
		pAkaza->Get_Transform()->Go_Straight(fTimeDelta, pAkaza->Get_NavigationCom());

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	CCollider*	pMyCollider = pAkaza->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{
		float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;
		
		_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);
		
		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pAkaza->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pAkaza->Get_NavigationCom());
	}
	
	RELEASE_INSTANCE(CGameInstance);
}

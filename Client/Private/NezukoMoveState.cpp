#include "stdafx.h"
#include "NezukoMoveState.h"
#include "NezukoIdleState.h"
#include "Nezuko.h"
#include "GameInstance.h"
#include "Characters.h"
#include "Layer.h"
#include "NezukoAtk_1_State.h"
#include "NezukoGuardState.h"
#include "NezukojumpState.h"
#include "NezukoDashState.h"
#include "NezukoMoveJumpState.h"
#include "NezukoSkill_FallCut.h"
#include "NezukoSkill_Move.h"
using namespace Nezuko;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CNezukoState * CMoveState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pNezuko->Get_i1P())
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
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					return new CSkill_FallCutState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(+200);
					pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_SKILL_MOVE);
					return new CSkill_MoveState(STATE_TYPE::TYPE_START);
				}
			}
		}

		if (pGameInstance->Key_Pressing(DIK_W)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LF);

				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}

				else
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RF);

				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LB);
				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RB);
				else if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}

		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_LEFT);
			else if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_RIGHT);

			else if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					return new CSkill_FallCutState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_SKILL_MOVE);
					return new CSkill_MoveState(STATE_TYPE::TYPE_START);
				}
			}
		}


		if (pGameInstance->Key_Pressing(DIK_UP)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LF);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RF);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LB);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // �� 
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RB);
				else if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START, fPositionY, 0.f);
				}
				else
					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_LEFT);
			else if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_RIGHT);
			else if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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

CNezukoState * CMoveState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			m_eStateType = CNezukoState::TYPE_LOOP;
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CNezukoState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	return nullptr;


}

CNezukoState * CMoveState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	Move(pNezuko, fTimeDelta);
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CMoveState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_MOVE_START);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_MOVE_START);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_MOVE_END);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_MOVE_END);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CNezuko* pNezuko)
{
}

void CMoveState::Move(CNezuko* pNezuko, _float fTimeDelta)
{
	_float fCamAngle = pNezuko->Get_CamAngle();
	
	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pNezuko->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pNezuko->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pNezuko->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pNezuko->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pNezuko->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pNezuko->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pNezuko->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pNezuko->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP)
		pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();
	CCollider*	pMyCollider = pNezuko->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{
		float fSpeed = pNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;
		
		_vector vPow = XMVector3Dot(pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);
		
		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	}
	
	RELEASE_INSTANCE(CGameInstance);
}

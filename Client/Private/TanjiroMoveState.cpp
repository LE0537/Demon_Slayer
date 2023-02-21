#include "stdafx.h"
#include "TanjiroMoveState.h"
#include "TanjiroIdlestate.h"
#include "TanjiroMoveJumpState.h"
#include "GameInstance.h"
#include "TanjiroAtk_1_State.h"
#include "Characters.h"
#include "Layer.h"
#include "TanjiroGuardState.h"
#include "TanjiroDashState.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroKaguraSkill_Move.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroChangeState.h"
#include "TanjiroKaguraAtk_1_State.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CTanjiroState * CMoveState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
		{
			if (pTanjiro->Get_KaguraMode())
				return new CAtk_1_KaguraState();
			else
				return new CAtk_1_State();
		}
		else if (pGameInstance->Key_Pressing(DIK_O) && pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (pTanjiro->Get_KaguraMode())
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
				}
				else
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::anim_skil);
						pTanjiro->Set_SkillBar(-200);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_IDLE);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_0);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_1);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_2);
						return new CSkill_WindMillState(TYPE_START);
					}
				}
			}
			else
			{
				if (pTanjiro->Get_KaguraMode())
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState();
					}
				}
				else
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CSkill_WaterMillState(STATE_TYPE::TYPE_START);
					}
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
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);

					return new CMoveJumpState(OBJDIR::DIR_LF, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RF);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RF, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_STRAIGHT);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, CTanjiroState::TYPE_START, fPositionY, 0.f);
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
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_LB, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RB);

				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RB, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Down(DIK_SPACE))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_BACK, CTanjiroState::TYPE_START, fPositionY, 0.f);
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
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LEFT, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_RIGHT);

			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RIGHT, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
		}
		else
			if (pTanjiro->Get_StoryKey() == true)
			{
				if (m_eDirection != DIR_STRAIGHT)
					return new CIdleState();
				else
					return new CIdleState(STATE_MOVE);
			}
			else
				return new CIdleState(STATE_MOVE);
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
		{
			if (pTanjiro->Get_KaguraMode())
				return new CAtk_1_KaguraState();
			else
				return new CAtk_1_State();
		}
		else if (pGameInstance->Key_Pressing(DIK_C) && pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (pTanjiro->Get_KaguraMode())
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
				}
				else
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL);
						pTanjiro->Set_SkillBar(-200);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_0);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_1);
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_2);
						return new CSkill_WindMillState(TYPE_START);
					}
				}
			}
			else
			{
				if (pTanjiro->Get_KaguraMode())
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState();
					}
				}
				else
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CSkill_WaterMillState(STATE_TYPE::TYPE_START);
					}
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
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_LF, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			{

				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RF);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RF, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_STRAIGHT);


				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, CTanjiroState::TYPE_START, fPositionY, 0.f);
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
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_LB, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
			{

				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RB);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_RB, CTanjiroState::TYPE_START, fPositionY, 0.f);
				}
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_BACK);

				if (pGameInstance->Key_Down(DIK_LCONTROL))
				{
					_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					_float fPositionY = XMVectorGetY(vPosition);
					return new CMoveJumpState(OBJDIR::DIR_BACK, CTanjiroState::TYPE_START, fPositionY, 0.f);
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
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LEFT, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_RIGHT);

			if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RIGHT, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CIdleState(STATE_MOVE);
		break;
	default:
		break;
	}




	return nullptr;
}

CTanjiroState * CMoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
		//	m_eStateType = CTanjiroState::TYPE_LOOP;
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CMoveState(m_eDirection, TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			//return new CMoveState(m_eDirection, TYPE_END);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	if(pTanjiro->Get_Stop())
		return new CIdleState(STATE_MOVE);

	return nullptr;
}

CTanjiroState * CMoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (m_eStateType != TYPE_END)
		Move(pTanjiro, fTimeDelta);

	if (m_eStateType == TYPE_END)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta* 0.85f);

	if (pTanjiro->Get_PlayerInfo().bChange)
	{
		return new CIdleState();
	}
	if (pTanjiro->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}
	if (pTanjiro->Get_PlayerInfo().iGuard < pTanjiro->Get_PlayerInfo().iMaxGuard)
	{
		pTanjiro->Set_GuardHp(1);
		if (pTanjiro->Get_PlayerInfo().iGuard > pTanjiro->Get_PlayerInfo().iMaxGuard)
			pTanjiro->Set_ResetGuardHp();
	}
	return nullptr;
}

void CMoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_START);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_MOVE_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_MOVE_START, 0.01f);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_LOOP);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_MOVE_LOOP, true);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_MOVE_LOOP, 0.01f);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_END);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_MOVE_END, 0.01f);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	}

	if (pTanjiro->Get_StoryKey() && (m_eDirection == OBJDIR::DIR_LEFT || m_eDirection == OBJDIR::DIR_RIGHT))
	{
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_IDLE);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
	}
}

void CMoveState::Exit(CTanjiro * pTanjiro)
{
	//pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_START);
}

void CMoveState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fCamAngle = pTanjiro->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		else
			pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());
		break;
	case Client::DIR_LEFT:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		else
			pTanjiro->Get_Transform()->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-3.f));
		break;
	case Client::DIR_RIGHT:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		else
			pTanjiro->Get_Transform()->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(3.f));
		break;
	case Client::DIR_BACK:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		else
		{
			pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());
			pTanjiro->Get_Transform()->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-3.f));
		}
		break;
	case Client::DIR_RF:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		else
		{
			pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());
			pTanjiro->Get_Transform()->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(3.f));
		}
		break;
	case Client::DIR_LB:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		if (!pTanjiro->Get_StoryKey())
			pTanjiro->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}
	if (!pTanjiro->Get_StoryKey())
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		if (m_eDirection != DIR_STOP)
			pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());
		CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
		CCollider*	pMyCollider = pTanjiro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
				pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pTanjiro->Get_NavigationCom());

		}

		RELEASE_INSTANCE(CGameInstance);
	}
}



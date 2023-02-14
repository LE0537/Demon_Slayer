#include "stdafx.h"
#include "RuiMoveState.h"
#include "RuiIdleState.h"
#include "Rui.h"
#include "GameInstance.h"
#include "Characters.h"
#include "Layer.h"
#include "RuiDashState.h"
#include "RuiAtk_1_State.h"
#include "RuiMoveJumpState.h"
#include "RuiGuardState.h"
#include "RuiSkill_ShootNet.h"
#include "RuiSkill_Sphere.h"
#include "RuiAdvSkill_MoveState.h"
#include "AiState.h"
#include "Effect_Manager.h"
using namespace Rui;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CRuiState * CMoveState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pRui->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_O) && pRui->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootNetState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_U))
		{
			if (pRui->Get_PlayerInfo().iFriendBar >= 500)
			{
				// 模备 霸捞瘤 别绰 内靛 持绢具窃
				pRui->Set_FriendSkillBar(-500);
				return new CAdvSkill_MoveState();
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else
			return new CIdleState(STATE_MOVE);
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_C) && pRui->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);

		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootNetState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_V))
		{
			if (pRui->Get_PlayerInfo().iFriendBar >= 500)
			{
				pRui->Set_FriendSkillBar(-500);
				return new CAdvSkill_MoveState();
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
					_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);

				return new CMoveJumpState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else
				return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		
		else
			return new CIdleState(STATE_MOVE);

		break;
	}




	return nullptr;
}

CRuiState * CMoveState::Tick(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_IsAIMode() == false)
	{
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CRuiState::TYPE_START:
				return new CMoveState(m_eDirection, TYPE_START);
				break;
			case Client::CRuiState::TYPE_LOOP:
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				return new CIdleState();
				break;
			case Client::CRuiState::TYPE_DEFAULT:
				break;
			default:
				break;
			}
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
		}
	}
	else
	{
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CRuiState::TYPE_START:
				return new CMoveState(m_eDirection, TYPE_START);

			}
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
		}
		return AIMove(pRui,  m_eDirection ,fTimeDelta);
	}


	return nullptr;
}

CRuiState * CMoveState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	Move(pRui, fTimeDelta);

	if (m_eStateType == TYPE_LOOP)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.1f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta* 0.85f);


	if (pRui->Get_PlayerInfo().bSub)
	{
		return new CIdleState();
	}
	if (pRui->Get_PlayerInfo().iGuard < pRui->Get_PlayerInfo().iMaxGuard)
	{
		pRui->Set_GuardHp(1);
		if (pRui->Get_PlayerInfo().iGuard > pRui->Get_PlayerInfo().iMaxGuard)
			pRui->Set_ResetGuardHp();
	}
	return nullptr;
}

void CMoveState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_MOVE_START);
		pRui->Set_AnimIndex(CRui::ANIM_MOVE_START);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_MOVE_START, true);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_MOVE_START, 0.01f);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_MOVE_END);
		pRui->Set_AnimIndex(CRui::ANIM_MOVE_END);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_MOVE_END);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_MOVE_END, 0.01f);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CRui* pRui)
{
}

void CMoveState::Move(CRui* pRui, _float fTimeDelta)
{
	_float fCamAngle = pRui->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pRui->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pRui->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pRui->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pRui->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pRui->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pRui->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pRui->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pRui->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP)
		pRui->Get_Transform()->Go_Straight(fTimeDelta, pRui->Get_NavigationCom());

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	CCollider*	pMyCollider = pRui->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{
		float fSpeed = pRui->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;
		
		_vector vPow = XMVector3Dot(pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);
		
		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		_vector vPlayerPosY = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];

		if (pRui->Get_NavigationCom()->Cheak_Cell(vPos))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pRui->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRui->Get_NavigationCom());
	}
	
	RELEASE_INSTANCE(CGameInstance);
}

CRuiState* CMoveState::AIMove(CRui * pRui,  OBJDIR eDir ,_float fTimeDelta)
{
	static _float fContinueTime = 0.f;
	static _bool bSetLook = false;

	if (bSetLook == false)
	{
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		bSetLook = true;
	}

	switch (eDir)
	{
	case Client::DIR_STRAIGHT:
		pRui->Get_Transform()->Go_Straight(fTimeDelta, pRui->Get_NavigationCom());
		break;
	case Client::DIR_LEFT:
		pRui->Get_Transform()->Go_Left(fTimeDelta, pRui->Get_NavigationCom());
		break;
	case Client::DIR_RIGHT:
		pRui->Get_Transform()->Go_Right(fTimeDelta, pRui->Get_NavigationCom());
		break;
	case Client::DIR_BACK:
		pRui->Get_Transform()->Go_Backward(fTimeDelta, pRui->Get_NavigationCom());
		break;
	}

	fContinueTime += fTimeDelta;

	if (fContinueTime >= 1.4f)
		return new CAiState();

	return nullptr;

}

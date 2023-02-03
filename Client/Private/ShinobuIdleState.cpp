#include "stdafx.h"
#include "ShinobuIdlestate.h"
#include "ShinobuMoveState.h"
#include "ShinobuJumpstate.h"
#include "GameInstance.h"

#include "ShinobuAtk_1_State.h"

#include "ShinobuGuardState.h"
#include "ShinobuGuardHitState.h"
#include "ShinobuSkill_Common.h"
#include "ShinobuChangeState.h"
#include "ShinobuSkill_Upper.h"
using namespace Shinobu;



CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
}

CShinobuState * CIdleState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pShinobu->Set_bGuard(false);
	if (!pShinobu->Get_PlayerInfo().bChange)
	{
		switch (pShinobu->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_W)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}

			else if (pGameInstance->Key_Pressing(DIK_D)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л 
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // аб
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_S)) // ©Л
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_SPACE)) // а║га
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Down(DIK_J))
			{
				return new CAtk_1_State();
			}
			else if (pGameInstance->Key_Pressing(DIK_O))
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_I))
			{
				if (pGameInstance->Key_Down(DIK_O))
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(STATE_TYPE::TYPE_START);
					}

				}
				else
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}

				}
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_UP)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}

			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л 
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ©Л
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_LCONTROL)) // а║га
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Down(DIK_Z))
			{
				return new CAtk_1_State();
			}
			else if (pGameInstance->Key_Pressing(DIK_C))
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (pGameInstance->Key_Down(DIK_C))
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(STATE_TYPE::TYPE_START);
					}

				}
				else
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}

				}
			}
			break;
		default:
			break;
		}
	}
	return nullptr;
}

CShinobuState * CIdleState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_PlayerInfo().bChange)
	{
		return new CChangeState(STATE_TYPE::TYPE_LOOP);
	}
	if (pShinobu->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}


	return nullptr;
}

CShinobuState * CIdleState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (ePreState == CShinobuState::STATE_JUMP)
	{
		pShinobu->Get_Model()->Play_Animation2(fTimeDelta);
	}
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	if (vPlayerY.m128_f32[1] > 0.f)
	{
		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	}
	return nullptr;
}

void CIdleState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_IDLE);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_IDLE);

	pShinobu->Get_Model()->Set_FrameNum(pShinobu->Get_AnimIndex(), 100);

}

void CIdleState::Exit(CShinobu* pShinobu)
{
}


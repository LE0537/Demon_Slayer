#include "stdafx.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
#include "GameInstance.h"
#include "AkazaAtk_1_State.h"
#include "AkazaJumpState.h"
#include "AkazaGuardState.h"
#include "AkazaHitState.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaTargetRushState.h"
using namespace Akaza;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CAkazaState * CIdleState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pAkaza->Set_bGuard(false);
	
	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_W)) // ╬у
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
			{
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // ╣з
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л 
			{
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // аб
		{
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
		{
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Down(DIK_SPACE)) // а║га
		{
			_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Pressing(DIK_O) && pAkaza->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_L))
			return new CTargetRushState(STATE_TYPE::TYPE_START);
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
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_U))
		{
			if (500 <= pAkaza->Get_PlayerInfo().iFriendBar)
			{
				pAkaza->Set_FriendSkillBar(-500.f);
				return new CAdvSkill_CommmonState(STATE_TYPE::TYPE_START);
			}
		}


		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_UP)) // ╬у
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
			{
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ╣з
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л 
			{
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
		{
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
		{
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();

		else if (pGameInstance->Key_Down(DIK_LCONTROL)) // а║га
		{
			_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}

		else if (pGameInstance->Key_Pressing(DIK_C) && pAkaza->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CTargetRushState(STATE_TYPE::TYPE_START);
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
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}	
		}
		else if (pGameInstance->Key_Pressing(DIK_V))
		{
			if (500 <= pAkaza->Get_PlayerInfo().iFriendBar)
			{
				pAkaza->Set_FriendSkillBar(-500.f);
				return new CAdvSkill_CommmonState(STATE_TYPE::TYPE_START);
			}
		}

	}

	return nullptr;
}

CAkazaState * CIdleState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_PlayerInfo().bSub)
	{
		pAkaza->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
	}


	return nullptr;
}

CAkazaState * CIdleState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	if (vPlayerY.m128_f32[1] > 0.f)
	{
		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	}
	if (pAkaza->Get_PlayerInfo().iGuard < pAkaza->Get_PlayerInfo().iMaxGuard)
	{
		pAkaza->Set_GuardHp(1);
		if (pAkaza->Get_PlayerInfo().iGuard > pAkaza->Get_PlayerInfo().iMaxGuard)
			pAkaza->Set_ResetGuardHp();
	}
	return nullptr;
}

void CIdleState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_IDLE);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_IDLE);

}

void CIdleState::Exit(CAkaza* pAkaza)
{
}

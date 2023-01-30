#include "stdafx.h"
#include "NezukoIdleState.h"
#include "Nezuko.h"

#include "GameInstance.h"
#include "NezukoAtk_1_State.h"
#include "NezukoMoveState.h"
#include "NezukoGuardState.h"
#include "NezukojumpState.h"


using namespace Nezuko;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CNezukoState * CIdleState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pNezuko->Set_bGuard(false);
	switch (pNezuko->Get_i1P())
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
			_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					//return new CSkill_DestoryState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					//return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
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
			_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}

		else if (pGameInstance->Key_Pressing(DIK_C))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					//return new CSkill_DestoryState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
				{
					pNezuko->Set_SkillBar(-200);
					//return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}	
		}

	}

	return nullptr;
}

CNezukoState * CIdleState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{



	return nullptr;
}

CNezukoState * CIdleState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CIdleState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_IDLE);
	pNezuko->Set_AnimIndex(CNezuko::ANIM_IDLE);

}

void CIdleState::Exit(CNezuko* pNezuko)
{
}

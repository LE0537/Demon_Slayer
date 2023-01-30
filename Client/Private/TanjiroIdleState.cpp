#include "stdafx.h"
#include "TanjiroIdlestate.h"
#include "TanjiroMoveState.h"
#include "TanjiroJumpstate.h"
#include "GameInstance.h"

#include "TanjiroAtk_1_State.h"
#include "TanjiroKaguraAtk_1_State.h"
#include "TanjiroGuardState.h"
#include "TanjiroGuardHitState.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroChangeState.h"
using namespace Tanjiro;



CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
}

CTanjiroState * CIdleState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pTanjiro->Set_bGuard(false);

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		////test
		//if (pGameInstance->Key_Down(DIK_R))
		//{
		//	return new CChangeState(STATE_TYPE::TYPE_START); // start -> up
		//}
		//if (pGameInstance->Key_Down(DIK_U))
		//{
		// _vector temp = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		// pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(XMVectorGetX(temp), 20.f, XMVectorGetZ(temp),1.f)); 

		//	return new CChangeState(STATE_TYPE::TYPE_LOOP); // loop -> down
		//}


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
			_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_J))
		{
			if (pTanjiro->Get_KaguraMode())
				return new CAtk_1_KaguraState();
			else
				return new CAtk_1_State();
		}
		else if (pGameInstance->Key_Pressing(DIK_O))
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
					pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL);
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CSkill_WindMillState();
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
						return new CKaguraSkill_CommonState();
					}
				}
				else
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}
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
			_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_Z))
		{
			if (pTanjiro->Get_KaguraMode())
				return new CAtk_1_KaguraState();
			else
				return new CAtk_1_State();
		}
		else if (pGameInstance->Key_Pressing(DIK_C))
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
					pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL);
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CSkill_WindMillState();
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
						return new CKaguraSkill_CommonState();
					}
				}
				else
				{
					if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
					{
						pTanjiro->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}
				}
			}
		}
		break;
	default:
		break;
	}

	return nullptr;
}

CTanjiroState * CIdleState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{



	return nullptr;
}

CTanjiroState * CIdleState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fTest = 0.f;
	fTest += fTimeDelta;


	if (ePreState == CTanjiroState::STATE_JUMP)
	{
		pTanjiro->Get_Model()->Play_Animation2(fTimeDelta);
	}
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CIdleState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_IDLE);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);

	pTanjiro->Get_Model()->Set_FrameNum(pTanjiro->Get_AnimIndex(), 100);

}

void CIdleState::Exit(CTanjiro * pTanjiro)
{
}


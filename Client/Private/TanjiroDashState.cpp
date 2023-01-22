#include "stdafx.h"
#include "TanjiroDashState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "TanjiroMoveState.h"

using namespace Tanjiro;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CTanjiroState * CDashState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CDashState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

		if (pGameInstance->Key_Pressing(DIK_L))
		{
			if (CTanjiro::ANIMID::ANIM_DASH_L_01 == pTanjiro->Get_AnimIndex() || CTanjiro::ANIMID::ANIM_DASH_R_01 == pTanjiro->Get_AnimIndex())
			{
				if (m_bSecondDash == false)
				{
					if (CTanjiro::ANIMID::ANIM_DASH_L_01 == pTanjiro->Get_AnimIndex())
						return new CDashState(OBJDIR::DIR_LF, true);
					else if (CTanjiro::ANIMID::ANIM_DASH_R_01 == pTanjiro->Get_AnimIndex())
						return new CDashState(OBJDIR::DIR_RF, true);
				}
			}
			else if (CTanjiro::ANIMID::ANIM_DASH_L_02 == pTanjiro->Get_AnimIndex() || CTanjiro::ANIMID::ANIM_DASH_R_02 == pTanjiro->Get_AnimIndex())
			{
				if (m_bSecondDash == false)
				{
					if (CTanjiro::ANIMID::ANIM_DASH_L_02 == pTanjiro->Get_AnimIndex())
						return new CDashState(OBJDIR::DIR_LF);
					else if (CTanjiro::ANIMID::ANIM_DASH_R_02 == pTanjiro->Get_AnimIndex())
						return new CDashState(OBJDIR::DIR_RF);
				}
				else
				{
					if (CTanjiro::ANIMID::ANIM_DASH_L_02 == pTanjiro->Get_AnimIndex())
						return new CDashState(OBJDIR::DIR_LEFT);
					else if (CTanjiro::ANIMID::ANIM_DASH_R_02 == pTanjiro->Get_AnimIndex())
						return new CDashState(OBJDIR::DIR_RIGHT);
				}
			}
			else
				return new CDashState(m_eDir);
		}

		else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
			return new CMoveState(m_eDir, STATE_TYPE::TYPE_LOOP);
		else
			return new CIdleState();
	}



	return nullptr;
}

CTanjiroState * CDashState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	Move(pTanjiro, fTimeDelta);
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_DASH;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront();
	pTanjiro->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	// 나중에 룩방향

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_F);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_F);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_F, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_F);
		break;
	case Client::DIR_LEFT:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		break;
	case Client::DIR_RIGHT:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		break;
	case Client::DIR_BACK:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_B);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_B);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_B, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_B);
		break;
	case Client::DIR_LF:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_02);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_02);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_02, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_02);
		break;
	case Client::DIR_RF:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_02);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_02);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_02, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_02);
		break;
	case Client::DIR_LB:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_02);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_02);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_02, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_02);
		break;
	case Client::DIR_RB:
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_02);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_02);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_02, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_02);
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}


}

void CDashState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
}

void CDashState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{



}




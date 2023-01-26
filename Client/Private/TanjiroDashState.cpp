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

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_L))
			{
				if (m_eDir == DIR_LEFT || m_eDir == DIR_RIGHT)
				{
					if (m_bSecondDash == false)
					{
						if (CTanjiro::ANIMID::ANIM_DASH_L_01 == pTanjiro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_LEFT_DASH, true);
						else if (CTanjiro::ANIMID::ANIM_DASH_R_01 == pTanjiro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_RIGHT_DASH, true);
					}
				}
				else if(m_eDir == DIR_LEFT_DASH || m_eDir == DIR_RIGHT_DASH)
				{
					if (m_bSecondDash == true)
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
		break;
	case 2:
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				if (m_eDir == DIR_LEFT || m_eDir == DIR_RIGHT)
				{
					if (m_bSecondDash == false)
					{
						if (CTanjiro::ANIMID::ANIM_DASH_L_01 == pTanjiro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_LEFT_DASH, true);
						else if (CTanjiro::ANIMID::ANIM_DASH_R_01 == pTanjiro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_RIGHT_DASH, true);
					}
				}
				else if (m_eDir == DIR_LEFT_DASH || m_eDir == DIR_RIGHT_DASH)
				{
					if (m_bSecondDash == true)
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

			else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_DOWN))
				return new CMoveState(m_eDir, STATE_TYPE::TYPE_LOOP);
			else
				return new CIdleState();
		}
		break;
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

	if (!m_bTrue)
	{
		_float fCamAngle = pTanjiro->Get_CamAngle();
		iIndex = pTanjiro->Get_iTargetIndex();
		switch (iIndex)
		{
		case 1:
			pTanjiro->Get_Transform()->Set_RotationY(0.f + fCamAngle);
			break;
		case 2:
			pTanjiro->Get_Transform()->Set_RotationY(180.f + fCamAngle);
			break;
		default:
			break;
		}
		m_bTrue = true;
	}

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_F);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_F);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_F, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_F);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_B);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_B);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_B, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_B);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_B);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_B);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_B, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_B);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_F);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_F);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_F, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_F);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_01);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_01, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_02);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_02);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_02, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_02);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_02);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_02);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_02, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_02);
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_R_02);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_R_02);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_R_02, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_R_02);
		}
		else if (iIndex == 2)
		{
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_DASH_L_02);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DASH_L_02);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_DASH_L_02, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DASH_L_02);
		}
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
	m_fTime += fTimeDelta;
	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.1f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.1f);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f);
			}
			else 
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.25f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f);
			}
			else
			{
				pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.1f);
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f);
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f);
		}
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}


}




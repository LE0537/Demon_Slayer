#include "stdafx.h"
#include "TanjiroDashState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "TanjiroMoveState.h"
#include "Effect_Manager.h"
using namespace Tanjiro;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CTanjiroState * CDashState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_W)) // ¾Õ
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);

					else
						return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RF);

					else
						return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_STRAIGHT);

					else
						return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_LOOP);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_S)) // µÚ
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);

					else
						return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì 
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RB);

					else
						return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_BACK);

					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_LOOP);
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
			{

				if (pGameInstance->Key_Pressing(DIK_L))
				{
					if (m_eDir == DIR_LEFT)
						return new CDashState(DIR_LEFT_DASH);
					else
						return new CDashState(DIR_LEFT);
				}

				else
					return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì
			{
				if (pGameInstance->Key_Pressing(DIK_L))
				{
					if (m_eDir == DIR_RIGHT)
						return new CDashState(DIR_RIGHT_DASH);
					else
						return new CDashState(DIR_RIGHT);
				}
				else
					return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
			}
			else
				return new CIdleState();
		}
		break;
	case 2:
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_UP)) // ¾Õ
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);

					else
						return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RF);

					else
						return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_STRAIGHT);

					else
						return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_LOOP);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // µÚ
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);

					else
						return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì 
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RB);

					else
						return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_BACK);

					else
						return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_LOOP);
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				{
					if (m_eDir == DIR_LEFT)
						return new CDashState(DIR_RIGHT_DASH);
					else
						return new CDashState(DIR_LEFT);
				}

				else
					return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				{
					if (m_eDir == DIR_RIGHT)
						return new CDashState(DIR_LEFT_DASH);
					else
						return new CDashState(DIR_RIGHT);
				}

				else
					return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
			}
			else
				return new CIdleState();
		}
		break;
	}

	return nullptr;
}

CTanjiroState * CDashState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

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
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else 
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
			}
			else
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
			else
				pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
		}
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}
	// ´ë½¬ ÀÌÆåÆ®
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK1_GROUND, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	Check_Coll(pTanjiro, fTimeDelta);
}

void CDashState::Check_Coll(CTanjiro * pTanjiro, _float fTimeDelta)
{

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
		{
			switch (m_eDir)
			{
			case Client::DIR_STRAIGHT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				break;
			case Client::DIR_LEFT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				break;
			case Client::DIR_BACK:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				break;
			case Client::DIR_LF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pTanjiro->Get_NavigationCom());
					}
					else
					{
						pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LEFT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
					else
						pTanjiro->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());
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

	}
}




#include "stdafx.h"
#include "NezukoDashState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "NezukoMoveState.h"

using namespace Nezuko;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CNezukoState * CDashState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pNezuko->Set_bGuard(false);
	switch (pNezuko->Get_i1P())
	{
	case 1:
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_W)) // 菊
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 谅
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);

					else
						return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 快
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

			else if (pGameInstance->Key_Pressing(DIK_S)) // 第
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 谅
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);

					else
						return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
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


			else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
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
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快
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
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);

					else
						return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
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

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);

					else
						return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
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


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				{
					if (m_eDir == DIR_LEFT)
						return new CDashState(DIR_LEFT_DASH);
					else
						return new CDashState(DIR_LEFT);
				}

				else
					return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
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
	}


	return nullptr;
}

CNezukoState * CDashState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{




	return nullptr;
}

CNezukoState * CDashState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	Move(pNezuko, fTimeDelta);
	pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_DASH;

	if (!m_bTrue)
	{
		_float fCamAngle = pNezuko->Get_CamAngle();
		iIndex = pNezuko->Get_iTargetIndex();
		switch (iIndex)
		{
		case 1:
			pNezuko->Get_Transform()->Set_RotationY(0.f + fCamAngle);
			break;
		case 2:
			pNezuko->Get_Transform()->Set_RotationY(180.f + fCamAngle);
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
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_F);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_F);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_F, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_F);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_B);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_B);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_B, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_B);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_B);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_B);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_B, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_B);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_F);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_F);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_F, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_F);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_01);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_01, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_02);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_02);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_02, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_02);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_02);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_02);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_02, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_02);
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_R_02);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_R_02);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_R_02, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_R_02);
		}
		else if (iIndex == 2)
		{
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_DASH_L_02);
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DASH_L_02);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_DASH_L_02, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_DASH_L_02);
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

void CDashState::Exit(CNezuko* pNezuko)
{
	pNezuko->Get_Model()->Reset_Anim(pNezuko->Get_AnimIndex());
}

void CDashState::Move(CNezuko* pNezuko, _float fTimeDelta)
{
	m_fTime += fTimeDelta;

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta, pNezuko->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pNezuko->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
			else
				pNezuko->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pNezuko->Get_NavigationCom());
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




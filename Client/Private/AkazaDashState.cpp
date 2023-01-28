#include "stdafx.h"
#include "AkazaDashState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "AkazaMoveState.h"

using namespace Akaza;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CAkazaState * CDashState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pAkaza->Set_bGuard(false);
	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

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
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

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

CAkazaState * CDashState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{




	return nullptr;
}

CAkazaState * CDashState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	Move(pAkaza, fTimeDelta);
	pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_DASH;

	if (!m_bTrue)
	{
		_float fCamAngle = pAkaza->Get_CamAngle();
		iIndex = pAkaza->Get_iTargetIndex();
		switch (iIndex)
		{
		case 1:
			pAkaza->Get_Transform()->Set_RotationY(0.f + fCamAngle);
			break;
		case 2:
			pAkaza->Get_Transform()->Set_RotationY(180.f + fCamAngle);
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
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_F);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_F);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_F, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_F);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_B);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_B);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_B, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_B);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_B);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_B);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_B, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_B);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_F);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_F);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_F, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_F);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_01);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_01);
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_01);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_01);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_01, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_01);
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_02);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_02);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_02, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_02);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_02);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_02);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_02, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_02);
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_R_02);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_R_02);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_R_02, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_R_02);
		}
		else if (iIndex == 2)
		{
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_DASH_L_02);
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DASH_L_02);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_DASH_L_02, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_DASH_L_02);
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

void CDashState::Exit(CAkaza* pAkaza)
{
	pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
}

void CDashState::Move(CAkaza* pAkaza, _float fTimeDelta)
{
	m_fTime += fTimeDelta;

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.4f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.4f);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.4f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.4f);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.4f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.4f);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.4f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.4f);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.2f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.2f);
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.2f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.2f);
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.2f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.2f);
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.2f);
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta);
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pAkaza->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.2f);
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.2f);
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.4f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.4f);
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Right(fTimeDelta * 0.4f);
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 1.5f);
			else
				pAkaza->Get_Transform()->Go_Left(fTimeDelta * 0.4f);
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




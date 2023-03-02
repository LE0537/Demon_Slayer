#include "stdafx.h"
#include "ShinobuDashState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "ShinobuMoveState.h"
#include "Effect_Manager.h"
#include "ShinobuSplSkrStartState.h"
using namespace Shinobu;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CShinobuState * CDashState::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pShinobu->Get_i1P())
	{
	case 1:
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_E))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pShinobu->Get_PlayerInfo().iUnicCount > 0)
				{
					pShinobu->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

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
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_RSHIFT))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pShinobu->Get_PlayerInfo().iUnicCount > 0)
				{
					pShinobu->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

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

CShinobuState * CDashState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	return nullptr;
}

CShinobuState * CDashState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	Move(pShinobu, fTimeDelta);
	pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_DASH;

	if (!m_bTrue)
	{
		_float fCamAngle = pShinobu->Get_CamAngle();
		iIndex = pShinobu->Get_iTargetIndex();
		switch (iIndex)
		{
		case 1:
			pShinobu->Get_Transform()->Set_RotationY(0.f + fCamAngle);
			break;
		case 2:
			pShinobu->Get_Transform()->Set_RotationY(180.f + fCamAngle);
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
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_F);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_F);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_F, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_F);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_B);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_B);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_B, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_B);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_B);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_B);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_B, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_B);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_F);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_F);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_F, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_F);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_01);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_01, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_01);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_0.wav"), g_fEffect);
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_02);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_02);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_02, 0.2f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_02);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_1.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_02);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_02);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_02, 0.2f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_02);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_1.wav"), g_fEffect);
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_R_02);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_R_02);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_R_02, 0.2f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_R_02);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_1.wav"), g_fEffect);
		}
		else if (iIndex == 2)
		{
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_DASH_L_02);
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DASH_L_02);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_DASH_L_02, 0.2f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_DASH_L_02);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Dash_1.wav"), g_fEffect);
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

void CDashState::Exit(CShinobu* pShinobu)
{
	pShinobu->Get_Model()->Reset_Anim(pShinobu->Get_AnimIndex());
}

void CDashState::Move(CShinobu* pShinobu, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else 
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
			}
			else
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
			else
				pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
		}
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	// ´ë½¬ ÀÌÆåÆ®
	if (!m_bEffect && m_eDir != DIR_STRAIGHT && m_eDir != DIR_BACK)
	{
		pEffectManger->Create_Effect(CEffect_Manager::EFF_DASH_TAN_MOVE, pShinobu);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_DASH_TAN_STOP, pShinobu);

		m_bEffect = true;
	}
	else if (!m_bEffect)
	{
		pEffectManger->Create_Effect(CEffect_Manager::EFF_DASH_TAN_MOVEFB, pShinobu);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_DASH_TAN_STOPFB, pShinobu);

		m_bEffect = true;
	}
	RELEASE_INSTANCE(CEffect_Manager);
	Check_Coll(pShinobu, fTimeDelta);


	if (pShinobu->Get_AnimIndex() == CShinobu::ANIM_DASH_R_02 ||
		pShinobu->Get_AnimIndex() == CShinobu::ANIM_DASH_L_02)
	{
		pShinobu->Get_Model()->Set_DurationTime(pShinobu->Get_AnimIndex(), 25.f);
	}

}

void CDashState::Check_Coll(CShinobu* pShinobu, _float fTimeDelta)
{

	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	CCollider*	pMyCollider = pShinobu->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{

		_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
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
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				break;
			case Client::DIR_LEFT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				break;
			case Client::DIR_BACK:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				break;
			case Client::DIR_LF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.25f, pShinobu->Get_NavigationCom());
					}
					else
					{
						pShinobu->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LEFT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Left(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pShinobu->Get_NavigationCom());
					else
						pShinobu->Get_Transform()->Go_Right(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());
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




#include "stdafx.h"
#include "RuiDadDashState.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "RuiDadMoveState.h"


using namespace RuiDad;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CRuiDadState * CDashState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CRuiDadState * CDashState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	
		if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
		{
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CIdleState();
		}
	



	return nullptr;
}

CRuiDadState * CDashState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{

	Move(pRuiDad, fTimeDelta);
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_DASH;

	if (!m_bTrue)
	{
		_float fCamAngle = pRuiDad->Get_CamAngle();
		iIndex = pRuiDad->Get_iTargetIndex();
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		//switch (iIndex)
		//{
		//case 1:
		//	pRuiDad->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		//	break;
		//case 2:
		//	pRuiDad->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		//	break;
		//default:
		//	break;
		//}
		m_bTrue = true;
	}


	_uint iRand = rand() % 2;

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_F);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_F);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_F, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_F);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_B);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_B);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_B, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_B);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_L, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_L);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_R, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_R);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
	
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_R, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_R);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_L, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_L);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_B);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_B);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_B, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_B);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_F);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_F);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_F, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_F);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_L, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_L);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_R, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_R);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_R, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_R);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_L, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_L);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_L, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_L);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_R, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_R);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_R);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_R, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_R);
		}
		else if (iIndex == 2)
		{
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_DASH_L);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_DASH_L, 0.01f);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_DASH_L);
		}
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_Dash.wav"), g_fEffect);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_0.wav"), g_fVoice);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_Dash_1.wav"), g_fVoice);
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}


}

void CDashState::Exit(CRuiDad* pRuiDad)
{
	pRuiDad->Get_Model()->Reset_Anim(pRuiDad->Get_AnimIndex());
}

void CDashState::Move(CRuiDad* pRuiDad, _float fTimeDelta)
{
	m_fTime += fTimeDelta;

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
			else
				pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
		}
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}
	Check_Coll(pRuiDad, fTimeDelta);
}

void CDashState::Check_Coll(CRuiDad* pRuiDad, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();
	CCollider*	pMyCollider = pRuiDad->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{

		_float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
			pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
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
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				break;
			case Client::DIR_LEFT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				break;
			case Client::DIR_BACK:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				break;
			case Client::DIR_LF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 2, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 2, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pRuiDad->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LEFT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pRuiDad->Get_NavigationCom());
					else
						pRuiDad->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pRuiDad->Get_NavigationCom());
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



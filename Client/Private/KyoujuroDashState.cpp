#include "stdafx.h"
#include "KyoujuroDashState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "KyoujuroMoveState.h"

using namespace Kyoujuro;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CKyoujuroState * CDashState::HandleInput(CKyoujuro* pKyoujuro)
{

	return nullptr;
}

CKyoujuroState * CDashState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_L))
			{
				if (m_eDir == DIR_LEFT || m_eDir == DIR_RIGHT)
				{
					if (m_bSecondDash == false)
					{
						if (CKyoujuro::ANIMID::ANIM_DASH_L_01 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_LEFT_DASH, true);
						else if (CKyoujuro::ANIMID::ANIM_DASH_R_01 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_RIGHT_DASH, true);
					}
				}
				else if (m_eDir == DIR_LEFT_DASH || m_eDir == DIR_RIGHT_DASH)
				{
					if (m_bSecondDash == true)
					{
						if (CKyoujuro::ANIMID::ANIM_DASH_L_02 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_LEFT);
						else if (CKyoujuro::ANIMID::ANIM_DASH_R_02 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_RIGHT);
					}
				}

				else
					return new CDashState(m_eDir);
			}

			else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_D))
				return new CMoveState(m_eDir, STATE_TYPE::TYPE_LOOP);
			else
				return new CIdleState();
		}
		break;
	case 2:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				if (m_eDir == DIR_LEFT || m_eDir == DIR_RIGHT)
				{
					if (m_bSecondDash == false)
					{
						if (CKyoujuro::ANIMID::ANIM_DASH_L_01 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_LEFT_DASH, true);
						else if (CKyoujuro::ANIMID::ANIM_DASH_R_01 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_RIGHT_DASH, true);
					}
				}
				else if (m_eDir == DIR_LEFT_DASH || m_eDir == DIR_RIGHT_DASH)
				{
					if (m_bSecondDash == true)
					{
						if (CKyoujuro::ANIMID::ANIM_DASH_L_02 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_LEFT);
						else if (CKyoujuro::ANIMID::ANIM_DASH_R_02 == pKyoujuro->Get_AnimIndex())
							return new CDashState(OBJDIR::DIR_RIGHT);
					}
				}

				else
					return new CDashState(m_eDir);
			}

			else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				return new CMoveState(m_eDir, STATE_TYPE::TYPE_LOOP);
			else
				return new CIdleState();
		}
		break;
	}



	return nullptr;
}

CKyoujuroState * CDashState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	Move(pKyoujuro, fTimeDelta);
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_DASH;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
	pKyoujuro->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	// ���߿� �����

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_F);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_F);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_F, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_F);
		break;
	case Client::DIR_LEFT:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_L_01);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_L_01);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_L_01, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_L_01);
		break;
	case Client::DIR_RIGHT:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_R_01);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_R_01);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_R_01, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_R_01);
		break;
	case Client::DIR_BACK:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_B);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_B);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_B, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_B);
		break;
	case Client::DIR_LF:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_L_01);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_L_01);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_L_01, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_L_01);
		break;
	case Client::DIR_RF:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_R_01);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_R_01);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_R_01, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_R_01);
		break;
	case Client::DIR_LB:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_L_01);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_L_01);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_L_01, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_L_01);
		break;
	case Client::DIR_RB:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_R_01);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_R_01);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_R_01, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_R_01);
		break;
	case Client::DIR_LEFT_DASH:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_L_02);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_L_02);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_L_02, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_L_02);
		break;
	case Client::DIR_RIGHT_DASH:
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_DASH_R_02);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DASH_R_02);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_DASH_R_02, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DASH_R_02);
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}


}

void CDashState::Exit(CKyoujuro* pKyoujuro)
{
	pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
}

void CDashState::Move(CKyoujuro* pKyoujuro, _float fTimeDelta)
{



}




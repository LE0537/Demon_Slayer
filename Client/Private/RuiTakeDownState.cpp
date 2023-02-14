#include "stdafx.h"
#include "RuiTakeDownState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"

using namespace Rui;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CRuiState * CTakeDownState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CTakeDownState::Tick(CRui* pRui, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pRui->Get_PlayerInfo().iUnicCount < 3 && pRui->Get_PlayerInfo().iUnicBar < pRui->Get_PlayerInfo().iUnicMaxBar)
			{
				pRui->Set_UnicBar(33);
				if (pRui->Get_PlayerInfo().iUnicBar >= pRui->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pRui->Get_PlayerInfo().iUnicCount < 3)
					{
						pRui->Reset_UnicBar();
						pRui->Set_UnicCount(1);
					}
					else
						pRui->Set_UnicBar(pRui->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}





	return nullptr;
}

CRuiState * CTakeDownState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pRui->Get_Model()->Reset_Anim(CRui::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pRui->Get_Model()->Reset_Anim(CRui::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pRui->Get_Model()->Reset_Anim(CRui::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_DOWN_COL);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		pRui->Set_GodMode(true);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_RETURN_1);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_RETURN_0);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.01f);
		break;
	case Client::CRuiState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	//_uint iRand = rand() % 4;

	//if (iRand == 0)
	//	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_1.wav"), fEFFECT);
	//else if (iRand == 1)
	//	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_2.wav"), fEFFECT);
	//else if (iRand == 2)
	//	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_3.wav"), fEFFECT);
	//else if (iRand == 3)
	//	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_4.wav"), fEFFECT);

	//if (iRand == 0)
	//	CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff1.wav"), fEFFECT);
	//else if (iRand == 1)
	//	CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff2.wav"), fEFFECT);
}




CRuiState * CTakeDownState::Jump(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	pRui->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pRui->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CRui* pRui)
{
	_int iHit = pRui->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_F);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_F);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pRui->Get_Atk2() == false)
		{
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_L);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_L);
			pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
			pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			pRui->Set_Atk2(true);
		}
		else if (pRui->Get_Atk2() == true)
		{
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_R);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_R);
			pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
			pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			pRui->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG2_G);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG2_G);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG2_F);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG2_F);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CRui* pRui)
{
}
void CTakeDownState::Exit(CRui* pRui)
{
	pRui->Set_HitTime(0.3f);
	//pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
}




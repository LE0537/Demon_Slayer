#include "stdafx.h"
#include "AkazaTakeDownState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"

using namespace Akaza;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CAkazaState * CTakeDownState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CTakeDownState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pAkaza->Get_PlayerInfo().iUnicCount < 3 && pAkaza->Get_PlayerInfo().iUnicBar < pAkaza->Get_PlayerInfo().iUnicMaxBar)
			{
				pAkaza->Set_UnicBar(33);
				if (pAkaza->Get_PlayerInfo().iUnicBar >= pAkaza->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pAkaza->Get_PlayerInfo().iUnicCount < 3)
					{
						pAkaza->Reset_UnicBar();
						pAkaza->Set_UnicCount(1);
					}
					else
						pAkaza->Set_UnicBar(pAkaza->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		_float fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fRatio = fCurrentDuration / fDuration;

		if (fRatio > 0.6f)
		{
			pAkaza->Set_GodMode(true);
		}

		break;
	}


	return nullptr;
}

CAkazaState * CTakeDownState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_DOWN_COL);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
	
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_RETURN_1);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_RETURN_0);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_CHANGE:
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




CAkazaState * CTakeDownState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;
	pAkaza->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pAkaza->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CAkaza* pAkaza)
{
	_int iHit = pAkaza->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_F);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_F);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pAkaza->Get_Atk2() == false)
		{
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_L);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_L);
			pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
			pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
			pAkaza->Set_Atk2(true);
		}
		else if (pAkaza->Get_Atk2() == true)
		{
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_R);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_R);
			pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
			pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
			pAkaza->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG2_G);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG2_G);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG2_F);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG2_F);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CAkaza* pAkaza)
{
}
void CTakeDownState::Exit(CAkaza* pAkaza)
{
	pAkaza->Set_HitTime(0.3f);
	//pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
}




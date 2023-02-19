#include "stdafx.h"
#include "TanjiroTakeDownState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"

using namespace Tanjiro;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CTanjiroState * CTakeDownState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CTakeDownState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pTanjiro->Get_PlayerInfo().iUnicCount < 3 && pTanjiro->Get_PlayerInfo().iUnicBar < pTanjiro->Get_PlayerInfo().iUnicMaxBar)
			{
				pTanjiro->Set_UnicBar(33);
				if (pTanjiro->Get_PlayerInfo().iUnicBar >= pTanjiro->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pTanjiro->Get_PlayerInfo().iUnicCount < 3)
					{
						pTanjiro->Reset_UnicBar();
						pTanjiro->Set_UnicCount(1);
					}
					else
						pTanjiro->Set_UnicBar(pTanjiro->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		_float fDuration = pTanjiro->Get_Model()->Get_Duration_Index(CTanjiro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fCurrentDuration = pTanjiro->Get_Model()->Get_CurrentTime_Index(CTanjiro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fRatio = fCurrentDuration / fDuration;

		if (fRatio > 0.6f)
		{
			pTanjiro->Set_GodMode(true);
		}

		break;
	}


	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}


	


	return nullptr;
}

CTanjiroState * CTakeDownState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_DOWN_COL);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);

		if (pTanjiro->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pTanjiro->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_RETURN_1);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_RETURN_0);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.01f);
		break;
	case Client::CTanjiroState::TYPE_CHANGE:
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




CTanjiroState * CTakeDownState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pTanjiro->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CTanjiro * pTanjiro)
{
	_int iHit = pTanjiro->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_F);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_F);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pTanjiro->Get_Atk2() == false)
		{
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_L);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_L);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			pTanjiro->Set_Atk2(true);
		}
		else if (pTanjiro->Get_Atk2() == true)
		{
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_R);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_R);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			pTanjiro->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG2_G);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG2_G);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG2_F);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG2_F);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CTanjiro * pTanjiro)
{
}
void CTakeDownState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Set_HitTime(0.3f);
	//pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
}




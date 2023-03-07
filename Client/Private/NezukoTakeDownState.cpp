#include "stdafx.h"
#include "NezukoTakeDownState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
using namespace Nezuko;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CNezukoState * CTakeDownState::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CTakeDownState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pNezuko->Get_PlayerInfo().iUnicCount < 3 && pNezuko->Get_PlayerInfo().iUnicBar < pNezuko->Get_PlayerInfo().iUnicMaxBar)
			{
				pNezuko->Set_UnicBar(33);
				pNezuko->Get_BattleTarget()->Set_UnicBar(16);
				if (pNezuko->Get_PlayerInfo().iUnicBar >= pNezuko->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pNezuko->Get_PlayerInfo().iUnicCount < 3)
					{
						pNezuko->Reset_UnicBar();
						pNezuko->Set_UnicCount(1);
					}
					else
						pNezuko->Set_UnicBar(pNezuko->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		_float fDuration = pNezuko->Get_Model()->Get_Duration_Index(CNezuko::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fCurrentDuration = pNezuko->Get_Model()->Get_CurrentTime_Index(CNezuko::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fRatio = fCurrentDuration / fDuration;

		if (fRatio > 0.6f)
		{
			pNezuko->Set_GodMode(true);
		}

		break;
	}

	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}


	return nullptr;
}

CNezukoState * CTakeDownState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_DOWN_COL);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		if (pNezuko->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pNezuko->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Nezuko_Hit_3.wav"), g_fVoice);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_RETURN_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_RETURN_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		break;
	default:
		break;
	}
}




CNezukoState * CTakeDownState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pNezuko->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CNezuko* pNezuko)
{
	_int iHit = pNezuko->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_F);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_F);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pNezuko->Get_Atk2() == false)
		{
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_L);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_L);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			pNezuko->Set_Atk2(true);
		}
		else if (pNezuko->Get_Atk2() == true)
		{
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_R);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_R);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			pNezuko->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG2_G);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG2_G);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG2_F);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG2_F);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CNezuko* pNezuko)
{
}
void CTakeDownState::Exit(CNezuko* pNezuko)
{
	pNezuko->Set_HitTime(0.3f);
	//pNezuko->Get_Model()->Reset_Anim(pNezuko->Get_AnimIndex());
}




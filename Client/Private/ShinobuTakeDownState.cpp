#include "stdafx.h"
#include "ShinobuTakeDownState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
using namespace Shinobu;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CShinobuState * CTakeDownState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CTakeDownState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pShinobu->Get_PlayerInfo().iUnicCount < 3 && pShinobu->Get_PlayerInfo().iUnicBar < pShinobu->Get_PlayerInfo().iUnicMaxBar)
			{
				pShinobu->Set_UnicBar(33);
				if (pShinobu->Get_PlayerInfo().iUnicBar >= pShinobu->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pShinobu->Get_PlayerInfo().iUnicCount < 3)
					{
						pShinobu->Reset_UnicBar();
						pShinobu->Set_UnicCount(1);
					}
					else
						pShinobu->Set_UnicBar(pShinobu->Get_PlayerInfo().iUnicMaxBar);
				}
				pShinobu->Get_BattleTarget()->Set_UnicBar(16);
				if (pShinobu->Get_BattleTarget()->Get_PlayerInfo().iUnicBar >= pShinobu->Get_BattleTarget()->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pShinobu->Get_BattleTarget()->Get_PlayerInfo().iUnicCount < 3)
					{
						pShinobu->Get_BattleTarget()->Reset_UnicBar();
						pShinobu->Get_BattleTarget()->Set_UnicCount(1);
					}
					else
						pShinobu->Get_BattleTarget()->Set_UnicBar(pShinobu->Get_BattleTarget()->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_LOOP:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}



	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		_float fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fRatio = fCurrentDuration / fDuration;

		if (fRatio > 0.6f)
		{
			pShinobu->Set_GodMode(true);
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

CShinobuState * CTakeDownState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_DOWN_COL);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		pShinobu->Set_GodMode(true);

		if (pShinobu->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pShinobu->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Shinobu_Hit_4.wav"), g_fVoice);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_RETURN_1);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_RETURN_0);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.01f);
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		break;
	default:
		break;
	}
}




CShinobuState * CTakeDownState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
	pShinobu->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pShinobu->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CShinobu* pShinobu)
{
	_int iHit = pShinobu->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_F);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_F);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pShinobu->Get_Atk2() == false)
		{
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_L);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_L);
			pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
			pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			pShinobu->Set_Atk2(true);
		}
		else if (pShinobu->Get_Atk2() == true)
		{
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_R);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_R);
			pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
			pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			pShinobu->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG2_G);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG2_G);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG2_F);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG2_F);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CShinobu* pShinobu)
{
}
void CTakeDownState::Exit(CShinobu* pShinobu)
{
	pShinobu->Set_HitTime(0.3f);
	//pShinobu->Get_Model()->Reset_Anim(pShinobu->Get_AnimIndex());
}




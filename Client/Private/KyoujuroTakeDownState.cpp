#include "stdafx.h"
#include "KyoujuroTakeDownState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
using namespace Kyoujuro;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CKyoujuroState * CTakeDownState::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CTakeDownState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3 && pKyoujuro->Get_PlayerInfo().iUnicBar < pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
			{
				pKyoujuro->Set_UnicBar(33);
				if (pKyoujuro->Get_PlayerInfo().iUnicBar >= pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3)
					{
						pKyoujuro->Reset_UnicBar();
						pKyoujuro->Set_UnicCount(1);
					}
					else
						pKyoujuro->Set_UnicBar(pKyoujuro->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CKyoujuroState::TYPE_END:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		_float fDuration = pKyoujuro->Get_Model()->Get_Duration_Index(CKyoujuro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fCurrentDuration = pKyoujuro->Get_Model()->Get_CurrentTime_Index(CKyoujuro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fRatio = fCurrentDuration / fDuration;

		if (fRatio > 0.6f)
		{
			pKyoujuro->Set_GodMode(true);
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

CKyoujuroState * CTakeDownState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_DOWN_COL);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		if (pKyoujuro->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pKyoujuro->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Hit_3.wav"), fVOICE);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_RETURN_1);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_RETURN_0);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.01f);
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		break;
	default:
		break;
	}

}




CKyoujuroState * CTakeDownState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pKyoujuro->Get_NavigationHeight().y;
	pKyoujuro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pKyoujuro->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CKyoujuro* pKyoujuro)
{
	_int iHit = pKyoujuro->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_F);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_F);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pKyoujuro->Get_Atk2() == false)
		{
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_L);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_L);
			pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
			pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			pKyoujuro->Set_Atk2(true);
		}
		else if (pKyoujuro->Get_Atk2() == true)
		{
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_R);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_R);
			pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
			pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			pKyoujuro->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG2_G);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG2_G);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG2_F);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG2_F);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CKyoujuro* pKyoujuro)
{
}
void CTakeDownState::Exit(CKyoujuro* pKyoujuro)
{
	pKyoujuro->Set_HitTime(0.3f);
	//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
}




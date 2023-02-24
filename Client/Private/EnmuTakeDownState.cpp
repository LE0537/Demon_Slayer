#include "stdafx.h"
#include "EnmuTakeDownState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
using namespace Enmu;

CTakeDownState::CTakeDownState(_float _fPow, _bool _bJump ,STATE_TYPE eType)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eType;
}

CEnmuState * CTakeDownState::HandleInput(CEnmu* pEnmu)
{
	return nullptr;
}

CEnmuState * CTakeDownState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		if (!m_bReset)
		{
			if (pEnmu->Get_PlayerInfo().iUnicCount < 3 && pEnmu->Get_PlayerInfo().iUnicBar < pEnmu->Get_PlayerInfo().iUnicMaxBar)
			{
				pEnmu->Set_UnicBar(33);
				if (pEnmu->Get_PlayerInfo().iUnicBar >= pEnmu->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pEnmu->Get_PlayerInfo().iUnicCount < 3)
					{
						pEnmu->Reset_UnicBar();
						pEnmu->Set_UnicCount(1);
					}
					else
						pEnmu->Set_UnicBar(pEnmu->Get_PlayerInfo().iUnicMaxBar);
				}
			}
			m_bReset = true;
		}
	}


	
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CTakeDownState(m_fPow, m_bJumpHit, TYPE_LOOP);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		_float fDuration = pEnmu->Get_Model()->Get_Duration_Index(CEnmu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fCurrentDuration = pEnmu->Get_Model()->Get_CurrentTime_Index(CEnmu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		_float fRatio = fCurrentDuration / fDuration;

		if (fRatio > 0.6f)
		{
			pEnmu->Set_GodMode(true);
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

CEnmuState * CTakeDownState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CTakeDownState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_HIT;


	//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
	//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_0);
	//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_1);

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_DOWN_COL);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_DOWN_COL);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		if (pEnmu->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pEnmu->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
	
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_RETURN_1);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	case Client::CEnmuState::TYPE_END:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_RETURN_0);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.01f);
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		break;
	default:
		break;
	}

}




CEnmuState * CTakeDownState::Jump(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;
	pEnmu->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pEnmu->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CTakeDownState::Set_HitState(CEnmu* pEnmu)
{
	_int iHit = pEnmu->Get_BattleTarget()->Get_TargetState();

	switch (iHit)
	{
	case 3: // atk 1
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_F);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_F);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	case 4: // atk 2
		if (pEnmu->Get_Atk2() == false)
		{
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_L);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_L);
			pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
			pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			pEnmu->Set_Atk2(true);
		}
		else if (pEnmu->Get_Atk2() == true)
		{
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_R);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_R);
			pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
			pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			pEnmu->Set_Atk2(false);
		}
		break;
	case 5: // atk 3
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG2_G);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG2_G);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	case 6: // atk 4
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG2_F);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG2_F);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	default:
		break;
	}




}
void CTakeDownState::Set_JumpHitState(CEnmu* pEnmu)
{
}
void CTakeDownState::Exit(CEnmu* pEnmu)
{
	pEnmu->Set_HitTime(0.3f);
	//pEnmu->Get_Model()->Reset_Anim(pEnmu->Get_AnimIndex());
}




#include "stdafx.h"
#include "RuiHitState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"

using namespace Rui;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump) 
{
}

CRuiState * CHitState::HandleInput(CRui* pRui)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(DIK_F3))
		return new CHitState(0.f);

	return nullptr;
}

CRuiState * CHitState::Tick(CRui* pRui, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pRui->Get_PlayerInfo().iUnicCount < 3 && pRui->Get_PlayerInfo().iUnicBar < pRui->Get_PlayerInfo().iUnicMaxBar)
		{
			pRui->Set_UnicBar(67);
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

	fHitTime += fTimeDelta * 60.f;

	//if (!m_bJumpHit)
	//{
	//	if (fHitTime <= 20.f)
	//		pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

	//	if (fHitTime >= 38.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//		pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

	//	if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_HIT))
	//	{
	//		pAkaza->Get_Model()->Set_End(CAkaza::ANIM_HIT);
	//		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
	//		return new CIdleState();
	//	}
	//}


	return nullptr;
}

CRuiState * CHitState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pRui, m_fJumpTime);
	}

	_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	if (m_bJumpHit == false)
	{
		if (pRui->Get_Model()->Get_CurrentFrame() <= 20)
		{
			pRui->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);
			pRui->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pRui->Get_NavigationCom());

			_vector vPlayerY = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
			if (vPlayerY.m128_f32[1] < 0)
				vPlayerY.m128_f32[1] = 0;

			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);

			if (pRui->Get_Model()->Get_CurrentFrame() == 19)
				return new CIdleState();
		}
	}
	else
	{
		if (pRui->Get_Model()->Get_CurrentFrame() <= 60)
		{
			if (!m_bTrun)
			{
				_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pRui->Get_NavigationCom()->Cheak_Cell(vPos))
					pRui->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pRui->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pRui->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pRui->Get_NavigationCom());
			}
		}

		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);


		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
		}

	}






	return nullptr;
}
	


void CHitState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT);
	pRui->Set_AnimIndex(CRui::ANIM_HIT);
	pRui->Get_Model()->Reset_Anim(CRui::ANIM_HIT);
	//pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	//pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.0f);

	if (m_bJumpHit == false)
	{
		pRui->Get_Model()->Set_FrameNum(pRui->Get_AnimIndex(), 100);
		//pAkaza->Get_Model()->Set_FrameTime(pAkaza->Get_AnimIndex(), 0, 20, 1.f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_HIT, 8, 20);
	}
	else
	{
		pRui->Get_Model()->Set_FrameNum(pRui->Get_AnimIndex(), 100);
		//pAkaza->Get_Model()->Set_FrameTime(pAkaza->Get_AnimIndex(), 0, 20, 1.f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_HIT, 25, 100);
	}


	pRui->Set_RuiHit(true);

	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Hit_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Hit_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Hit_3.wav"), fEFFECT);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_Hit_4.wav"), fEFFECT);

}
CRuiState * CHitState::Jump(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	pRui->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 8.f;
	static _float fGravity = 14.f;


	_vector      vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


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
void CHitState::Exit(CRui* pRui)
{
	pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_HIT, 0, 100);
	pRui->Set_HitTime(0.5f);
	//pRui->Set_RuiHit(false);
}




#include "stdafx.h"
#include "TanjiroHitState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"

using namespace Tanjiro;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CTanjiroState * CHitState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CHitState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pTanjiro->Get_PlayerInfo().iUnicCount < 3 && pTanjiro->Get_PlayerInfo().iUnicBar < pTanjiro->Get_PlayerInfo().iUnicMaxBar)
		{
			pTanjiro->Set_UnicBar(67);
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
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_HIT);
		m_bReset = true;
	}
	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pTanjiro->Get_NavigationCom());

		_vector vPlayerY = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	
		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
		if (fHitTime >= 35.f)
			return new CIdleState();
	}
	else if (m_bJumpHit)
	{
		if (fHitTime <= 35.f)
		{
			if (!m_bTrun)
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
					pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pTanjiro->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pTanjiro->Get_NavigationCom());
			}
		}
		if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_HIT))
		{
			pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_HIT);
			//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT);

			pTanjiro->Get_Model()->Set_CurrentAnimIndex(58);
			pTanjiro->Get_Model()->Set_Loop(58);
			pTanjiro->Get_Model()->Set_LinearTime(58, 0.01f);
			pTanjiro->Set_bGuard(true);
		}

		if (pTanjiro->Get_Model()->Get_End(58))
		{
			pTanjiro->Get_Model()->Reset_Anim(58);
			pTanjiro->Get_Model()->Set_End(58);
			pTanjiro->Set_bGuard(false);
			return new CIdleState();
		}
	}



	return nullptr;
}

CTanjiroState * CHitState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pTanjiro, m_fJumpTime);
	}
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);




	return nullptr;
}

void CHitState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT);

	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_3.wav"), fEFFECT);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_4.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff2.wav"), fEFFECT);
}
CTanjiroState * CHitState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
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
void CHitState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Set_HitTime(0.3f);
}




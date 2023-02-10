#include "stdafx.h"
#include "KyoujuroHitState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"

using namespace Kyoujuro;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump) 
{
}

CKyoujuroState * CHitState::HandleInput(CKyoujuro* pKyoujuro)
{

	return nullptr;
}

CKyoujuroState * CHitState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3 && pKyoujuro->Get_PlayerInfo().iUnicBar < pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
		{
			pKyoujuro->Set_UnicBar(67);
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
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_HIT);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_HIT);
		m_bReset = true;
	}
	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());

		_vector vPlayerY = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);

		if (fHitTime >= 38.f)
			return new CIdleState();
	}
	else if(m_bJumpHit)
	{
		if (fHitTime <= 35.f)
		{
			if (!m_bTrun)
			{
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pKyoujuro->Get_NavigationCom()->Cheak_Cell(vPos))
					pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());
			}
		}
		if(pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_HIT))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_HIT);
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(46);
			pKyoujuro->Get_Model()->Set_Loop(46);
			pKyoujuro->Get_Model()->Set_LinearTime(46, 0.01f);
			pKyoujuro->Set_bGuard(true);
		}

		if (pKyoujuro->Get_Model()->Get_End(46))
		{
			pKyoujuro->Get_Model()->Reset_Anim(46);
			pKyoujuro->Get_Model()->Set_End(46);
			pKyoujuro->Set_bGuard(false);
			return new CIdleState();
		}
	}
	return nullptr;
}

CKyoujuroState * CHitState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pKyoujuro, m_fJumpTime);
	}
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);

	return nullptr;
}

void CHitState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT);

	_uint iRand = rand() % 3;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_Hit_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_Hit_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_Hit_3.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff2.wav"), fEFFECT);
}
CKyoujuroState * CHitState::Jump(CKyoujuro * pKyoujuro, _float fTimeDelta)
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
void CHitState::Exit(CKyoujuro* pKyoujuro)
{
	pKyoujuro->Set_HitTime(0.5f);
}




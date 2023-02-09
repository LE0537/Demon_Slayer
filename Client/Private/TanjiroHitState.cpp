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
}
CTanjiroState * CHitState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 12.5f;
	static _float fGravity = 15.f;


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




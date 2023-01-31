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
			pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());

		if(pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_HIT))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_HIT);
			pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_HIT);
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
}
CKyoujuroState * CHitState::Jump(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 12.5f;
	static _float fGravity = 15.f;


	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
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
	pKyoujuro->Set_HitTime(0.2f);
}




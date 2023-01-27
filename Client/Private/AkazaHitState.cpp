#include "stdafx.h"
#include "AkazaHitState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"

using namespace Akaza;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump) 
{
}

CAkazaState * CHitState::HandleInput(CAkaza* pAkaza)
{

	return nullptr;
}

CAkazaState * CHitState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (!m_bReset)
	{
		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_HIT);
		m_bReset = true;
	}
	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

		if (fHitTime >= 38.f)
			return new CIdleState();
	}
	else if(m_bJumpHit)
	{
		if (fHitTime <= 35.f)
			pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

		if(pAkaza->Get_Model()->Get_End(CAkaza::ANIM_HIT))
		{
			pAkaza->Get_Model()->Set_End(CAkaza::ANIM_HIT);
			pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
			return new CIdleState();
		}
	}
	return nullptr;
}

CAkazaState * CHitState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pAkaza, m_fJumpTime);
	}
	pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.1f);

	return nullptr;
}

void CHitState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT);

}
CAkazaState * CHitState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 12.5f;
	static _float fGravity = 15.f;


	_vector      vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Exit(CAkaza* pAkaza)
{
	
}




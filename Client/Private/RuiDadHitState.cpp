#include "stdafx.h"
#include "RuiDadHitState.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"

using namespace RuiDad;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump) 
{
}

CRuiDadState * CHitState::HandleInput(CRuiDad* pRuiDad)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(DIK_F3))
		return new CHitState(0.f);

	return nullptr;
}

CRuiDadState * CHitState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	//if (!m_bReset)
	//{
	//	pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
	//	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_HIT);
	//	m_bReset = true;
	//}

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

CRuiDadState * CHitState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{

	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pRuiDad, m_fJumpTime);
	}

	_vector vPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	if (m_bJumpHit == false)
	{
		
			pRuiDad->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);
			pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pRuiDad->Get_NavigationCom());

			_vector vPlayerY = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
			if (vPlayerY.m128_f32[1] < 0)
				vPlayerY.m128_f32[1] = 0;

			pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);

			if (pRuiDad->Get_Model()->Get_CurrentFrame() == 19)
			{
				if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
				{
					pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
					return new CIdleState();

				}

				
			}
				
		
	}
	else
	{
	
			if (!m_bTrun)
			{
				_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
					pRuiDad->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pRuiDad->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pRuiDad->Get_NavigationCom());
			}
		}

		pRuiDad->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);


		if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
		{
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
		}

	

	return nullptr;
}
	


void CHitState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_HIT;


	pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIM_HIT);
	pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIM_HIT_FULL);

	//pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	//pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.0f);

	if (m_bJumpHit == false)
	{
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_HIT);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_HIT);
	}
	else
	{
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_HIT_FULL);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_HIT_FULL);
	}

	pRuiDad->Set_RuiDadHit(true);

	//pRuiDad->Set_RuiHit(true);


}
CRuiDadState * CHitState::Jump(CRuiDad* pRuiDad, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 8.f;
	static _float fGravity = 14.f;


	_vector      vPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	

	return nullptr;
}
void CHitState::Exit(CRuiDad* pRuiDad)
{
	
	pRuiDad->Set_HitTime(0.5f);
	//pRuiDad->Set_RuiHit(false);
}




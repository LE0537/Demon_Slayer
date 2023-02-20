#include "stdafx.h"
#include "RuiDadHitState.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
using namespace RuiDad;

CHitState::CHitState(_float _fPow, STATE_TYPE eTYPE, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
	m_eStateType = eTYPE;
}

CRuiDadState * CHitState::HandleInput(CRuiDad* pRuiDad)
{


	return nullptr;
}

CRuiDadState * CHitState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 3.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}

	if (pRuiDad->Get_AnimIndex() == CRuiDad::ANIM_HIT_FULL)
	{
		if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_HIT_FULL))
		{
			m_bRuiDadDead = true;
			pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_HIT_FULL);
		}
	}

	else
	{

		if (pRuiDad->Get_PlayerInfo().iAccDamage >= 250)
		{

			if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
			{
				switch (m_eStateType)
				{
				case Client::CRuiDadState::TYPE_START:
					pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
					if (!m_bEffect)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

						pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_FREE, pRuiDad);

						RELEASE_INSTANCE(CEffect_Manager);
						m_bEffect = true;
					}
					return new CHitState(0.f, TYPE_LOOP);
					break;
				case Client::CRuiDadState::TYPE_LOOP:
					pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
					return new CHitState(0.f, TYPE_END);
					break;
				case Client::CRuiDadState::TYPE_END:
					pRuiDad->Set_AccDmg();
					pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
					pRuiDad->Set_RuiDadHit(false);
					return new CIdleState();
					break;

				}
				pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());

			}

		}
		else
		{
			return new CIdleState();
		}
	}

	return nullptr;
}

CRuiDadState * CHitState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	//_vector vPosition = XMVector3TransformCoord(XMVectorSet(0.f, -3.f, 0.f, 1.f), pRuiDad->Get_Transform()->Get_WorldMatrix());
	//pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pRuiDad, m_fJumpTime);
	//}

	if(m_bRuiDadDead== false)
		pRuiDad->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}



void CHitState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_HIT;

	//CHierarchyNode*		pSocket = pRuiDad->Get_Model()->Get_BonePtr("C_Spine_3");

	//pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	//pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.0f);
	


	if (pRuiDad->Get_PlayerInfo().iHp <= 0)
	{
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_HIT_FULL);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_HIT_FULL);
		pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex());
		pRuiDad->Get_Model()->Set_LinearTime(pRuiDad->Get_AnimIndex(), 0.2f);
		pRuiDad->Set_GodMode(true);
		g_bDeathTime = true;
	}
	else
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_HIT_FREE_0);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_HIT_FREE_0);
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_HIT_FREE_0, false);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_HIT_FREE_0, 0.01f);
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_HIT_FREE_1);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_HIT_FREE_1);
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_HIT_FREE_1, false);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_HIT_FREE_1, 0.01f);
			break;
		case Client::CRuiDadState::TYPE_END:
			pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_HIT_FREE_2);
			pRuiDad->Set_AnimIndex(CRuiDad::ANIM_HIT_FREE_2);
			pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_HIT_FREE_2, false);
			pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_HIT_FREE_2, 0.01f);
			break;
		}
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

	//pRuiDad->Set_HitTime(0.5f);
	//pRuiDad->Set_RuiHit(false);
}




#include "stdafx.h"
#include "TanjiroTargetRushState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
using namespace Tanjiro;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CTargetRushState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CTargetRushState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START: 
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP: 
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());;
			break;
		case Client::CTanjiroState::TYPE_END: 
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_LOOP:
		Move(pTanjiro, fTimeDelta);

		if (m_bNextAnim == true)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f);
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CTanjiroState * CTargetRushState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_TARGET_RUSH_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_TARGET_RUSH_0);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_0);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_TARGET_RUSH_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_TARGET_RUSH_1);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pTanjiro);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_TARGET_RUSH_2);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_TARGET_RUSH_2);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_1, false);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_TARGET_RUSH_2);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CTanjiro * pTanjiro)
{
}

void CTargetRushState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pTanjiro->Get_SphereCollider()->Collision(pTanjiro->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


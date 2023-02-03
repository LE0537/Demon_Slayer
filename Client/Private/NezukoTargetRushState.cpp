#include "stdafx.h"
#include "NezukoTargetRushState.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"
#include "NezukoMoveState.h"
using namespace Nezuko;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CTargetRushState::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CTargetRushState::Tick(CNezuko * pNezuko, _float fTimeDelta)
{

	pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START: 
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP: 
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());;
			break;
		case Client::CNezukoState::TYPE_END: 
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_LOOP:
		Move(pNezuko, fTimeDelta);

		if (m_bNextAnim == true)
		{
			pNezuko->Get_BattleTarget()->Take_Damage(0.1f);
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CNezukoState * CTargetRushState::Late_Tick(CNezuko * pNezuko, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CNezuko * pNezuko)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_TARGET_RUSH_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_TARGET_RUSH_0);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_TARGET_RUSH_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_TARGET_RUSH_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pNezuko);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_TARGET_RUSH_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_TARGET_RUSH_2);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_1, false);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_TARGET_RUSH_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CNezuko * pNezuko)
{
}

void CTargetRushState::Move(CNezuko * pNezuko, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pNezuko->Get_SphereCollider()->Collision(pNezuko->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CNezuko * pNezuko)
{
	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


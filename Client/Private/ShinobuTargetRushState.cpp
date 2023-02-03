#include "stdafx.h"
#include "ShinobuTargetRushState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
using namespace Shinobu;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CShinobuState * CTargetRushState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CTargetRushState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START: 
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_LOOP: 
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());;
			break;
		case Client::CShinobuState::TYPE_END: 
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_LOOP:
		Move(pShinobu, fTimeDelta);

		if (m_bNextAnim == true)
		{
			pShinobu->Get_BattleTarget()->Take_Damage(0.1f);
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CShinobuState * CTargetRushState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_TARGET_RUSH_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_TARGET_RUSH_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_TARGET_RUSH_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_TARGET_RUSH_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_TARGET_RUSH_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pShinobu);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_TARGET_RUSH_2);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_TARGET_RUSH_2);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_TARGET_RUSH_1, false);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_TARGET_RUSH_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CShinobu* pShinobu)
{
}

void CTargetRushState::Move(CShinobu* pShinobu, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pShinobu->Get_SphereCollider()->Collision(pShinobu->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
	}
	else
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CShinobu* pShinobu)
{
	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


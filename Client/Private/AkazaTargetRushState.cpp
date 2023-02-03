#include "stdafx.h"
#include "AkazaTargetRushState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
using namespace Akaza;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CTargetRushState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CTargetRushState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START: 
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP: 
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());;
			break;
		case Client::CAkazaState::TYPE_END: 
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_LOOP:
		Move(pAkaza, fTimeDelta);

		if (m_bNextAnim == true)
		{
			pAkaza->Get_BattleTarget()->Take_Damage(0.1f);
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CAkazaState * CTargetRushState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_TARGET_RUSH_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_TARGET_RUSH_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_TARGET_RUSH_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_TARGET_RUSH_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pAkaza);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_TARGET_RUSH_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_TARGET_RUSH_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_1, false);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_TARGET_RUSH_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CAkaza* pAkaza)
{
}

void CTargetRushState::Move(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pAkaza->Get_SphereCollider()->Collision(pAkaza->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CAkaza* pAkaza)
{
	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


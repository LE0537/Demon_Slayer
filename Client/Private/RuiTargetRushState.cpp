#include "stdafx.h"
#include "RuiTargetRushState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "RuiMoveState.h"
using namespace Rui;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CTargetRushState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CTargetRushState::Tick(CRui* pRui, _float fTimeDelta)
{

	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START: 
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP: 
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());;
			break;
		case Client::CRuiState::TYPE_END: 
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_LOOP:
		Move(pRui, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CRuiState * CTargetRushState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CRui* pRui)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_TARGET_RUSH_0);
		pRui->Set_AnimIndex(CRui::ANIM_TARGET_RUSH_0);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_TARGET_RUSH_1);
		pRui->Set_AnimIndex(CRui::ANIM_TARGET_RUSH_1);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pRui);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_TARGET_RUSH_2);
		pRui->Set_AnimIndex(CRui::ANIM_TARGET_RUSH_2);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_1, false);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_TARGET_RUSH_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CRui* pRui)
{
}

void CTargetRushState::Move(CRui* pRui, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pRui->Get_SphereCollider()->Collision(pRui->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pRui->Get_BattleTarget()->Get_PlayerInfo().bGuard)
		{
			pRui->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			pRui->Get_BattleTarget()->Take_Damage(0.3f, false);
		}
	}
	else
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CRui* pRui)
{
	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


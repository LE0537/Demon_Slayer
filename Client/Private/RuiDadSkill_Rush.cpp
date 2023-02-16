#include "stdafx.h"
#include "RuiDadSkill_Rush.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
using namespace RuiDad;


CSkill_RushState::CSkill_RushState(STATE_TYPE eType)
{
	m_eStateType = eType;

}

CRuiDadState * CSkill_RushState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	


	return nullptr;
}

CRuiDadState * CSkill_RushState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	//pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex());
	pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CSkill_RushState(CRuiDadState::TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			//return new CSkill_JumpDropState(CRuiDadState::TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		Move(pRuiDad, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CSkill_RushState(CRuiDadState::TYPE_END);
		}
		break;
	case Client::CRuiDadState::TYPE_END:
		break;
	case Client::CRuiDadState::TYPE_DEFAULT:
		break;
	case Client::CRuiDadState::TYPE_CHANGE:
		break;
	default:
		break;
	}

	return nullptr;
}

CRuiDadState * CSkill_RushState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_RushState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL2_0);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL2_0, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL2_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL2_0);

		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL2_1);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL2_1, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL2_1);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL2_1, true);
		Initialize_value(pRuiDad);
		break;
	case Client::CRuiDadState::TYPE_END:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL2_2);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL2_2, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL2_2);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL2_2);
		break;
	}




}

void CSkill_RushState::Exit(CRuiDad* pRuiDad)
{

}

void CSkill_RushState::Move(CRuiDad * pRuiDad, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	if (fDistance <= 3.f) // юс╫ц
	{
		m_bNextAnim = true;
	}
	else
		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	//if (pRuiDad->Get_SphereCollider()->Collision(pRuiDad->Get_BattleTarget()->Get_SphereCollider()))
	//{
	//	m_bNextAnim = true;
	//	_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	//	pRuiDad->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

	//	if (pRuiDad->Get_BattleTarget()->Get_PlayerInfo().bGuard && pRuiDad->Get_BattleTarget()->Get_PlayerInfo().iGuard > 0)
	//	{
	//		pRuiDad->Get_BattleTarget()->Get_GuardHit(0);
	//	}
	//	else
	//	{
	//		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	//		dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
	//		RELEASE_INSTANCE(CGameInstance);
	//		pRuiDad->Get_BattleTarget()->Take_Damage(0.3f, false);
	//	}
	//}
	//else
	//	pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


}

void CSkill_RushState::Initialize_value(CRuiDad * pRuiDad)
{
	m_vPosition.x = XMVectorGetX(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRuiDad->Set_NavigationHeight(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


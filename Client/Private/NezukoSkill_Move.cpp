#include "stdafx.h"
#include "NezukoSkill_Move.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Nezuko;


CSkill_MoveState::CSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;
	m_bNextAnim = false;
	m_fTime = 0.f;
	RELEASE_INSTANCE(CGameInstance);

}

CNezukoState * CSkill_MoveState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CNezukoState * CSkill_MoveState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();



	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		//return new CIdleStat();
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			if (m_bNextAnim == true)
				return new CSkill_MoveState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (m_bNextAnim == true)
				return new CSkill_MoveState(TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		default:
			break;
		}
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		Increase_Height(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		// °øÁß¿¡¼­ ³»·ÁÂï±âÀü »ìÂ¦ µô·¹ÀÌ
		m_fTime += fTimeDelta;
		if(m_fTime >= 0.11f)
			Fall_Height(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_END:
		break;
	default:
		break;
	}

	return nullptr;
}

CNezukoState * CSkill_MoveState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if(m_eStateType == TYPE_START)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.1f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_MoveState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_MOVE;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_MOVE_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_MOVE_0, 0.2f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_MOVE_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 1.f;
		m_vVelocity.y = 1.f;
		m_vVelocity.z = 1.f;
		m_fGravity = 0.f;

		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;

		m_fHeight = XMVectorGetY(m_vTargetPosition);
		m_fHeight += 5.f;

		m_vTargetPosition = XMVectorSetY(m_vTargetPosition, m_fHeight);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_MOVE_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_MOVE_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_MOVE_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex(),true);
		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;

		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_MOVE_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_MOVE_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_MOVE_2);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex(), false);
		break;
	default:
		break;
	}

	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pNezuko->Get_Transform()->Set_PlayerLookAt(vTargetPosition);
	
}

void CSkill_MoveState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

void CSkill_MoveState::Fall_Height(CNezuko * pNezuko, _float fTimeDelta)
{
	
	static _float fGravity = -400.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

	
}

void CSkill_MoveState::Increase_Height(CNezuko * pNezuko, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = 9.8f;

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;


	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 3.5f * fTimeDelta;

	m_vPosition.y += XMVectorGetY(m_vTargetPosition) *	 m_vVelocity.y * 0.5f * fTimeDelta;

	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z * 3.5f * fTimeDelta;


	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (XMVectorGetY(vCurrentPos) > m_fHeight)
	{
	
		//pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);




}


#include "stdafx.h"
#include "NezukoSkill_FallCut.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Nezuko;


CSkill_FallCutState::CSkill_FallCutState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;

}

CNezukoState * CSkill_FallCutState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CNezukoState * CSkill_FallCutState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_FallCutState(CNezukoState::TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (m_bNextAnim == true)
			{		
				return new CSkill_FallCutState(CNezukoState::TYPE_END);
			}

			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			if (vPos.m128_f32[1] > 0.f)
			{
				vPos.m128_f32[1] = 0.f;
				pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
			pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}


	if (m_eStateType == STATE_TYPE::TYPE_START)
		Increase_Height(pNezuko, fTimeDelta);
	if (m_eStateType == STATE_TYPE::TYPE_LOOP)
		Fall_Height(pNezuko, fTimeDelta);

	return nullptr;
}

CNezukoState * CSkill_FallCutState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();

	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pNezuko->Get_Transform()->LookAt(vLooAt);
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pNezuko->Get_SphereCollider();
	m_fMove += fTimeDelta;

	/*if (m_eStateType == STATE_TYPE::TYPE_START)
	{
		if(!m_bLook)
			pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());
	}*/
	if (m_eStateType == STATE_TYPE::TYPE_LOOP)
	{
		_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 2.5f; //추가
		vCollPos.m128_f32[1] = 0.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
	
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-80);
					m_pTarget->Take_Damage(0.8f, true);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}
		}
	}
	/*if (pMyCollider2->Collision(pTargetCollider))
	{
		_float fSpeed = pNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 2.f;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pNezuko->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pNezuko->Get_NavigationCom());
	}*/

	if (!m_bLook)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 2.f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_FallCutState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_FALLCUT;



	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		_vector vLook = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		pNezuko->Get_Transform()->Set_PlayerLookAt(vLook);

		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_0, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		if (!m_bLookPos)
		{
			_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		/*	_float fDist = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
			if (fDist < 10.f)
				m_bLook = true;*/
			m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
			m_bLookPos = true;
		}
		m_fGravity = 15.81f;
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_2);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
	/*	if (!m_bLookPos)
		{
			_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fDist = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
			if (fDist < 10.f)
				m_bLook = true;
			m_bLookPos = true;
		}*/
		break;
	}

}

void CSkill_FallCutState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

CNezukoState * CSkill_FallCutState::Increase_Height(CNezuko * pNezuko, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = 100.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{

		//pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}


	return nullptr;
}

CNezukoState * CSkill_FallCutState::Fall_Height(CNezuko * pNezuko, _float fTimeDelta)
{

	static _float fGravity = -500.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity * fTimeDelta;
	if (!m_bLook)
	{
		vPosition.x += XMVectorGetX(m_vTargetPosition) * 50.f * fTimeDelta;
		vPosition.y += XMVectorGetY(m_vTargetPosition) * -fVelocity * fTimeDelta;
		vPosition.z += XMVectorGetZ(m_vTargetPosition) * 50.f * fTimeDelta;
	}
	else
		vPosition.y += XMVectorGetY(m_vTargetPosition) * -fVelocity * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, vPosition.x);
		vecPos = XMVectorSetY(vecPos, vPosition.y);
		vecPos = XMVectorSetZ(vecPos, vPosition.z);
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		else
		{
			vecPos = XMVectorSetX(vecPos, XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			vecPos = XMVectorSetZ(vecPos, XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		}
		m_bNextAnim = true;
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		else
		{
			vecPos = XMVectorSetX(vecPos, XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			vecPos = XMVectorSetZ(vecPos, XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		}
	}

	return nullptr;
}


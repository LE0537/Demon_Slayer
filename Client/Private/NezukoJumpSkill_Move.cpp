#include "stdafx.h"
#include "NezukoJumpSkill_Move.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Nezuko;


CJumpSkill_MoveState::CJumpSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;
	m_fTime = 0.f;
	m_fMove = 0.1f;
	RELEASE_INSTANCE(CGameInstance);

}

CNezukoState * CJumpSkill_MoveState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CNezukoState * CJumpSkill_MoveState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		//return new CIdleStat();
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	
			return new CJumpSkill_MoveState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (m_bNextAnim == true)
				return new CJumpSkill_MoveState(TYPE_END);
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
			Move(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		// 공중에서 내려찍기전 살짝 딜레이
			Jump(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_END:
		break;
	default:
		break;
	}

	return nullptr;
}

CNezukoState * CJumpSkill_MoveState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();

	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pNezuko->Get_Transform()->LookAt(vLooAt);
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pNezuko->Get_SphereCollider();
	m_fMove += fTimeDelta;

	if (m_eStateType == STATE_TYPE::TYPE_START)
	{
		if (m_bLook)
			pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.1f, pNezuko->Get_NavigationCom());
	}
	if (m_eStateType == STATE_TYPE::TYPE_LOOP)
	{
		_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

		if (m_fMove > 0.1f && m_iHit < 2)
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
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-30);
					m_pTarget->Take_Damage(0.2f, false);
					pNezuko->Set_Combo(1);
					pNezuko->Set_ComboTime(0.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				++m_iHit;
			}
		}
	}
	if (m_eStateType == STATE_TYPE::TYPE_END)
	{
		_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
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
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-30);
					m_pTarget->Take_Damage(0.8f, true);
					pNezuko->Set_Combo(1);
					pNezuko->Set_ComboTime(0.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}
		}
	}
	pNezuko->Get_Model()->Play_Animation(fTimeDelta * 2.f);

	return nullptr;
}

void CJumpSkill_MoveState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_JUMP;

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

		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_MOVE_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_MOVE_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_MOVE_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex(), true);
		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		

		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_MOVE_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_MOVE_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_MOVE_2);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex(), false);
		break;
	
	}
		_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		pNezuko->Get_Transform()->Set_PlayerLookAt(vTargetPosition);
}





void CJumpSkill_MoveState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

void CJumpSkill_MoveState::Jump(CNezuko * pNezuko, _float fTimeDelta)
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
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpSkill_MoveState::Move(CNezuko * pNezuko, _float fTimeDelta)
{
	static _float fGravity = 19.8f;

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));


	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x  * fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  * fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (fDistance <= 3.f)
	{

	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


}

void CJumpSkill_MoveState::Initialize_value(CNezuko * pNezuko)
{

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;


	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

}

void CJumpSkill_MoveState::Initialize_MoveValue(CNezuko * pNezuko)
{
	m_vMovePosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vMovePosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vMovePosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vMoveVelocity.x = 1.f;
	m_vMoveVelocity.y = 0.f;
	m_vMoveVelocity.z = 1.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fMoveOriginPosY = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}




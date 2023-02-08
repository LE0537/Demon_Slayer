#include "stdafx.h"
#include "AkazaJumpMoveAttackState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Akaza;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CAkazaState * CJumpMoveAttackState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CJumpMoveAttackState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START: // 공격 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP: // 떨어지는 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());;
			break;
		case Client::CAkazaState::TYPE_END: // 착지 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_LOOP:
		Jump(pAkaza, fTimeDelta);

		if (m_bNextAnim == true)
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CAkazaState * CJumpMoveAttackState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (m_eStateType == CAkazaState::TYPE_END)
	{
		CCharacters* m_pTarget = pAkaza->Get_BattleTarget();


		_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pAkaza->Get_SphereCollider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-30);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-pAkaza->Get_PlayerInfo().iDmg);
					m_pTarget->Take_Damage(0.5f, false);
					pAkaza->Set_Combo(1);
					pAkaza->Set_ComboTime(0.f);
				}

				_int iDest = rand() % 5;
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				switch (iDest)
				{
				case 0:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
					break;
				case 1:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
					break;
				case 2:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
					break;
				case 3:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
					break;
				case 4:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
					break;
				default:
					break;
				}


				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
				pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pAkaza->Get_NavigationCom());
		}

	}
	if (m_eStateType == TYPE_END)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else if(m_eStateType == TYPE_START)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 2.f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_MOVE_ATTACK_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_MOVE_ATTACK_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pAkaza);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_MOVE_ATTACK_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}

void CJumpMoveAttackState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fGravity = 200.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;
	 


	if (m_bRange == true)
	{
		//m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition);
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition) * 10.f;
		//m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition);
	}
	else if (m_bRange == false)
	{
		m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition)* 6.f;
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition)* 10.f;
		m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition)* 6.f;
	}


	_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vecPos))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vecPos))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CAkaza* pAkaza)
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


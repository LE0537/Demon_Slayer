#include "stdafx.h"
#include "ShinobuJumpMoveAttackState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
#include "Effect_Manager.h"
#include "ShinobuSkill_Move.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Shinobu;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType, _bool bContinueSkill)
{
	m_eStateType = eType;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);

	m_bContinue = bContinueSkill;

}

CShinobuState * CJumpMoveAttackState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CJumpMoveAttackState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_EffectTime(fTimeDelta);

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START: // 공격 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP, m_bContinue);
			break;
		case Client::CShinobuState::TYPE_LOOP: // 떨어지는 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			break;
		case Client::CShinobuState::TYPE_END: // 착지 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			if (m_bContinue == true)
			{
				return new CSkill_MoveState(TYPE_LOOP);
			}
			else
				return new CIdleState(STATE_JUMP);
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_LOOP:
		Jump(pShinobu, fTimeDelta);

		if (m_bNextAnim == true)
			return new CJumpMoveAttackState(TYPE_END, m_bContinue);
		break;
	}



	return nullptr;
}

CShinobuState * CJumpMoveAttackState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == CShinobuState::TYPE_END)
	{
		CCharacters* m_pTarget = pShinobu->Get_BattleTarget();

		_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pShinobu->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(_int(-pShinobu->Get_PlayerInfo().iDmg * pShinobu->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.5f, false);
					pShinobu->Set_Combo(1);
					pShinobu->Set_ComboTime(0.f);
				}

				CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Hit_JumpMoveAttack.wav"), fEFFECT);

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
			_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
				pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pShinobu->Get_NavigationCom());
		}

	}
	if (m_eStateType == TYPE_END)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	if (!m_bEffect &&
		m_eStateType == TYPE_LOOP)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SHINOBU_ATK_JUMP_MOVE_FOL, pShinobu);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SHINOBU_ATK_JUMP_MOVE_NONFOL, pShinobu);
		
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CJumpMoveAttackState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_MOVE_ATTACK_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_JumpMoveAttack.wav"), fEFFECT);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_MOVE_ATTACK_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pShinobu);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_MOVE_ATTACK_2);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

void CJumpMoveAttackState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	pShinobu->Get_Transform()->Set_Jump(true);

	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;
	 


	if (m_bRange == true)
	{
		//m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition);
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition) * 5.f;
		//m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition);
	}
	else if (m_bRange == false)
	{
		m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition)* 3.f;
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition)* 5.f;
		m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition)* 3.f;
	}


	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		pShinobu->Get_Transform()->Set_Jump(false);
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vecPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vecPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CShinobu* pShinobu)
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


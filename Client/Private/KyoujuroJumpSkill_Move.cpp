#include "stdafx.h"
#include "KyoujuroJumpSkill_Move.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "KyoujuroJumpState.h"
using namespace Kyoujuro;


CJumpSkill_MoveState::CJumpSkill_MoveState(_float fPositionY)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_KyoujuroJumpSkill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;


	m_fPositionY = fPositionY;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CJumpSkill_MoveState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CJumpSkill_MoveState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{

	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	pKyojuro->Set_JumpState(true);



	m_fTime += fTimeDelta;

	if (m_fTime <= 0.5f)
		Move(pKyojuro, fTimeDelta * 1.5f);
	else
	{
		if (m_bFinishAnim == false)
		{
			if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
			{
				pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
				m_bFinishAnim = true;
			
			}
		}
	}

	if (m_bFinishAnim == true)
	{
		Jump(pKyojuro, fTimeDelta);
	}

	static _bool bSetting = false;

	if (pKyojuro->Get_Model()->Get_CurrentTime() >= 25.f && bSetting == false)
	{
		m_vPosition.x = XMVectorGetX(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.y = 10.f;

		bSetting = true;
	}
		
	if (pKyojuro->Get_Model()->Get_CurrentTime() >= 30.f)
	{
		m_bFinishAnim = true;
	}



	return nullptr;
}

CKyoujuroState * CJumpSkill_MoveState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{


	CCharacters* m_pTarget = pKyojuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pKyojuro->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;
	
	if (m_fMove > 0.3f)
	{
		m_fDelay += fTimeDelta;
		_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pKyojuro->Get_SphereCollider();
		if (m_fDelay > 0.2f && m_iHit < 4)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-pKyojuro->Get_PlayerInfo().iDmg);
					m_pTarget->Take_Damage(0.3f, false);
					pKyojuro->Set_Combo(1);
					pKyojuro->Set_ComboTime(0.f);
				}
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				++m_iHit;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 2.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyojuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyojuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyojuro->Get_NavigationCom());
		}
	}


	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_3KIEN_START, pKyojuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_3KIEN_STARTFIRE, pKyojuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_3KIEN_MAIN, pKyojuro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	if (m_bNextAnim == true)
	{
		m_pCollBox->Set_Dead();
		return new CIdleState();
	}

	return nullptr;
}

void CJumpSkill_MoveState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_JUMPMOVE;


	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Get_Model()->Set_FrameNum(CKyoujuro::ANIM_SKILL_JUMPMOVE, 100);



	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;
	m_vPosition.y = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

}

void CJumpSkill_MoveState::Exit(CKyoujuro * pKyojuro)
{
	m_pCollBox->Set_Dead();
}

CKyoujuroState * CJumpSkill_MoveState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	static _float fY = (m_vPosition.y - m_fPositionY) / 2.f;
	CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
	if (!m_bLook)
	{
		m_bLook = true;
		_vector vLookAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(vLookAt);
	}
	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vDistance = vMyPosition - vTargetPosition;
	_float fDistance = XMVectorGetX(XMVector3Length(vDistance));

	if(fDistance >= 4.f) 
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta, pKyoujuro->Get_NavigationCom());

	
	_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vPosition = XMVectorSetY(vPosition, fY);

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);



	return nullptr;
}

CKyoujuroState* CJumpSkill_MoveState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fGravity = -34.81f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fPositionY)
	{
		vPosition.y = m_fPositionY;
		fVelocity = m_fPositionY;

		_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;

}


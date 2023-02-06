#include "stdafx.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CSkill_DestoryState::CSkill_DestoryState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CAkazaState * CSkill_DestoryState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	


	return nullptr;
}

CAkazaState * CSkill_DestoryState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_float fDist = 0.f;

	if (m_eStateType == CAkazaState::TYPE_START)
	{
		_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vLook2 = vPos - vTarget;

		_float fDist = XMVectorGetX(XMVector3Length(vLook2));

		if (fDist < 18.f)
		{
			if (pAkaza->Get_Model()->Get_CurrentTime() > 30.f)
			{
				pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
				_vector vTargetPos = vPos - vTarget;

				vTarget += XMVector3Normalize(vTargetPos) * 2.f;
				vTarget.m128_f32[1] = 0.f;
				if(pAkaza->Get_NavigationCom()->Cheak_Cell(vTarget))
					pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTarget);
				//pAkaza->Get_NavigationCom()->Find_CurrentCellIndex(vTarget);
				return new CSkill_DestoryState(CAkazaState::TYPE_END);
			}
		}
	}

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;
}

CAkazaState * CSkill_DestoryState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

	if (m_eStateType == CAkazaState::TYPE_END)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		if (m_fMove > 0.15f && m_fMove < 0.45f)
		{
			if (m_iHit < 1)
			{
				_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				vCollPos += XMVector3Normalize(vCollLook) * 2.f; //추가
				vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_float4 vTagetPos;
					XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					vPos.m128_f32[1] = 0.f;
					m_pTarget->Get_Transform()->LookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						m_pTarget->Set_Hp(-20);
						m_pTarget->Take_Damage(0.1f, false);
						pAkaza->Set_Combo(1);
						pAkaza->Set_ComboTime(0.f);
					}

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

					RELEASE_INSTANCE(CEffect_Manager);

					++m_iHit;
				}
			}


			CCollider*	pMyCollider = pAkaza->Get_SphereCollider();
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				_vector vPlayerPosY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
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
		else if (m_fMove > 0.5f && m_fMove < 1.f)
		{
			if (!m_bHit)
			{
				_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				vCollPos += XMVector3Normalize(vCollLook) * 4.f; //추가
				vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						m_pTarget->Set_Hp(-30);
						m_pTarget->Take_Damage(0.5f, true);
						pAkaza->Set_Combo(1);
						pAkaza->Set_ComboTime(0.f);
					}

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

					RELEASE_INSTANCE(CEffect_Manager);

					m_bHit = true;
				}
			}
		}

		RELEASE_INSTANCE(CGameInstance);
	}
	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);
		break;
	}

	return nullptr;
}

void CSkill_DestoryState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_DESTROY;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_DESTROY_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_DESTROY_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_DESTROY_0);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_DESTROY_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_DESTROY_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_DESTROY_1);
		break;
	}

}

void CSkill_DestoryState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}


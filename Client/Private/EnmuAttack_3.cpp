#include "stdafx.h"
#include "EnmuAttack_3.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuIdleState.h"
#include "Camera_Dynamic.h"

using namespace Enmu;

CEnmuAttack3::CEnmuAttack3(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_eStateType = eType;
}

CEnmuState * CEnmuAttack3::HandleInput(CEnmu* pEnmu)
{
	return nullptr;
}

CEnmuState * CEnmuAttack3::Tick(CEnmu* pEnmu, _float fTimeDelta)
{


	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_ATTACK_3))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_ATTACK_3);
		return new CIdleState();
	}






	return nullptr;
}

CEnmuState * CEnmuAttack3::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pEnmu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//vLooAt.m128_f32[1] = 0.f;
	pEnmu->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fMove += fTimeDelta;
	m_fComboDelay += fTimeDelta;
	if (m_fMove > 0.3f && m_fMove <= 0.86f)
	{
		_vector vCollPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] += 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pEnmu->Get_SphereCollider();
		if (m_fComboDelay > 0.15f && m_iHit < 4)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pEnmu->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pEnmu->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(-pEnmu->Get_PlayerInfo().iDmg * pEnmu->Get_PlayerInfo().fPowerUp);

					if (m_bIsCreate == false)
					{
						m_pTarget->Take_Damage(0.0f, false);
						//m_bIsCreate = true;
					}
					pEnmu->Set_Combo(1);
					pEnmu->Set_ComboTime(0.f);
				}
				if (pEnmu->Get_BattleTarget()->Get_GodMode() == false)
				{
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
					m_fComboDelay = 0.f;
					++m_iHit;
				}
			}
		}
	}
	else if (m_fMove >= 1.05f && m_fMove < 1.15f)
	{
		_vector vCollPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] += 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pEnmu->Get_SphereCollider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pEnmu->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pEnmu->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(-pEnmu->Get_PlayerInfo().iDmg * pEnmu->Get_PlayerInfo().fPowerUp * 1.5f);

					if (m_bIsCreate == false)
					{
						m_pTarget->Player_TakeDown(0.2f, false);
						//m_pTarget->Take_Damage(0.0f, false);
						m_bIsCreate = true;
					}
					pEnmu->Set_Combo(1);
					pEnmu->Set_ComboTime(0.f);
				}
				if (pEnmu->Get_BattleTarget()->Get_GodMode() == false)
				{
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

		}

		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pEnmu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			if (g_iLevel != LEVEL_BATTLEENMU)
				vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPos))
				pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pEnmu->Get_NavigationCom());
		}
	}


	pEnmu->Get_Model()->Play_Animation(fTimeDelta);
	return nullptr;
}

void CEnmuAttack3::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_SKILL2;
	pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_ATTACK_3);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_ATTACK_3);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_ATTACK_3);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_ATTACK_3, 0.01f);
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());

}

void CEnmuAttack3::Exit(CEnmu* pEnmu)
{
	m_pCollBox->Set_Dead();
}


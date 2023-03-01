#include "stdafx.h"
#include "EnmuAttack_2.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuAttack_3.h"
#include "EnmuIdleState.h"
#include "SoundMgr.h"

using namespace Enmu;

CEnmuAttack2::CEnmuAttack2(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_eStateType = eType;
}

CEnmuState * CEnmuAttack2::HandleInput(CEnmu* pEnmu)
{
	return nullptr;
}

CEnmuState * CEnmuAttack2::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_CurrentTime_Index(CEnmu::ANIMID::ANIM_ATTACK_2) >= 30.f)
	{
		_vector vEnmuPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vEnmuPosition));

		if (pEnmu->Get_TargetState() == 7 || fDistance <= 10.f)
		{
			pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_ATTACK_2);
			return new CEnmuAttack3(TYPE_START);
		}
	}

	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_ATTACK_2))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_ATTACK_2);
		return new CIdleState();
	}


	return nullptr;
}

CEnmuState * CEnmuAttack2::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pEnmu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pEnmu->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fMove += fTimeDelta;
	if (m_fMove < 0.7f)
	{
		if(m_fMove < 0.4f)
			pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pEnmu->Get_NavigationCom());
		if (m_iHit < 1)
		{

			_vector vCollPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
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
					m_pTarget->Set_Hp(_int(-pEnmu->Get_PlayerInfo().iDmg * pEnmu->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.f, false);
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

					++m_iHit;
				}
			}
		}
	}
	else if (m_fMove < 0.8f && m_fMove >= 0.75f)
	{
		if (!m_bHit)
		{
			_vector vCollPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
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
					m_pTarget->Set_Hp(_int(-pEnmu->Get_PlayerInfo().iDmg * pEnmu->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.f, false);
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
	}

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMU_ATK2_1, pEnmu);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMU_ATK2_2, pEnmu);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	return nullptr;
}

void CEnmuAttack2::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_ATK_2;
	pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_ATTACK_2);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_ATTACK_2);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_ATTACK_2);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_ATTACK_2, 0.01f);
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Attack_1.wav"), fEFFECT);

	_uint iRand = rand() % 2;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Attack1_0.wav"), fVOICE);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Attack1_1.wav"), fVOICE);
}

void CEnmuAttack2::Exit(CEnmu* pEnmu)
{
	m_pCollBox->Set_Dead();
}


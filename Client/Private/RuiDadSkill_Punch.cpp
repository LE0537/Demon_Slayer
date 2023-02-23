#include "stdafx.h"
#include "RuiDadSkill_Punch.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace RuiDad;


CSkill_PunchState::CSkill_PunchState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_GAMEPLAY, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState * CSkill_PunchState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	


	return nullptr;
}

CRuiDadState * CSkill_PunchState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	pRuiDad->Set_EffectTime(fTimeDelta);
	//pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex());

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
		return new CIdleState();
	}

	return nullptr;
}

CRuiDadState * CSkill_PunchState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();

	m_fMove += fTimeDelta;

	if (m_fMove < 0.9f)
	{
		if (m_fMove > 0.4f && m_fMove < 0.9f)
			pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());

		_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
		if (m_fMove > 0.5f && m_fMove < 0.7f && m_iHit == 0)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-100 * pRuiDad->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(_int(-100 * pRuiDad->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.5f, true);
					pRuiDad->Set_Combo(1);
					pRuiDad->Set_ComboTime(0.f);
				}


				if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
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

		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
				pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pRuiDad->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRuiDad->Get_NavigationCom());
		}
	}

	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_SWING_GROUND, pRuiDad);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_SWING_SLASH, pRuiDad);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	return nullptr;
}

void CSkill_PunchState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL1);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL1, 0.01f);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL1);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL1);
	pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CSkill_PunchState::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead();
}


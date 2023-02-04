#include "stdafx.h"
#include "RuiAdvSkill_CommonState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "Effect_Manager.h"
using namespace Rui;

CAdvSkill_CommonState::CAdvSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiSphere"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
			return;
	
	RELEASE_INSTANCE(CGameInstance);
}

CRuiState * CAdvSkill_CommonState::HandleInput(CRui * pRui)
{
	return nullptr;
}

CRuiState * CAdvSkill_CommonState::Tick(CRui * pRui, _float fTimeDelta)
{
	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

		return new CIdleState();

	}



	return nullptr;
}

CRuiState * CAdvSkill_CommonState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	if (!m_bTargetPos)
	{
		m_bTargetPos = true;
	
		vLooAt.m128_f32[1] = 1.f;
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vLooAt); //추가

	}
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	if (m_fMove > 0.6f)
		m_fDelay += fTimeDelta;
	if (m_fDelay > 0.1f && m_iHit < 4)
	{

		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

			if (m_pTarget->Get_PlayerInfo().bGuard)
			{
				m_pTarget->Get_GuardHit(0);
			}
			else
			{
				m_pTarget->Set_Hp(-15);
				m_pTarget->Take_Damage(0.1f, false);
			}

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

			RELEASE_INSTANCE(CEffect_Manager);
			m_fDelay = 0.f;
			++m_iHit;
		}

	}
	if (m_fMove > 1.1f && !m_bHit && m_fMove < 1.3f)
	{

		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

			if (m_pTarget->Get_PlayerInfo().bGuard)
			{
				m_pTarget->Get_GuardHit(0);
			}
			else
			{
				m_pTarget->Set_Hp(-30);
				m_pTarget->Take_Damage(0.1f, true);
			}

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

			RELEASE_INSTANCE(CEffect_Manager);
		
			m_bHit = true;
		}

	}



	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CAdvSkill_CommonState::Enter(CRui * pRui)
{
	m_eStateId = STATE_ID::STATE_ADVSKILL_COMMON;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_ADVSKILL_COMMON);
	pRui->Set_AnimIndex(CRui::ANIM_ADVSKILL_COMMON);

	pRui->Get_Model()->Set_Loop(CRui::ANIM_ADVSKILL_COMMON);
	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CAdvSkill_CommonState::Exit(CRui * pRui)
{
	m_pCollBox->Set_Dead(); //추가
}



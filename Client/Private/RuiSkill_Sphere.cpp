#include "stdafx.h"
#include "RuiSkill_Sphere.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Rui;


CSkill_SphereState::CSkill_SphereState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	if (m_eStateType != CRuiState::TYPE_START)
	{
		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiSphere"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
			return;
	}
	RELEASE_INSTANCE(CGameInstance);

}

CRuiState * CSkill_SphereState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CRuiState * CSkill_SphereState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_SphereState(CRuiState::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_SphereState(CRuiState::TYPE_END);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	return nullptr;
}

CRuiState * CSkill_SphereState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);
	if (m_eStateType == CRuiState::TYPE_LOOP)
	{
		CCharacters* m_pTarget = pRui->Get_BattleTarget();

		m_fMove += fTimeDelta;
		
		
		if (m_fMove > 0.05f && m_iHit < 5)
		{
			_vector vCollPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가

			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
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
					m_pTarget->Set_Hp(-10);
					m_pTarget->Take_Damage(0.f, false);
					pRui->Set_Combo(1);
					pRui->Set_ComboTime(0.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);
				m_fMove = 0.f;
				++m_iHit;
			}

		}
			
		
	}
	if (m_eStateType == CRuiState::TYPE_END)
	{
		CCharacters* m_pTarget = pRui->Get_BattleTarget();

		m_fMove += fTimeDelta;

		_vector vCollPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가

		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		if (m_fMove > 0.3f && !m_bHit)
		{
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
					m_pTarget->Take_Damage(0.5f, true);
					pRui->Set_Combo(1);
					pRui->Set_ComboTime(0.f);
				}
	
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
	
				RELEASE_INSTANCE(CEffect_Manager);
	
				m_bHit = true;
			}
		}
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.3f);

	if (!m_bEffect && m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_START, pRui);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_MAIN, pRui);
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CSkill_SphereState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_SKILL_SPHERE;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SPHERE_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SPHERE_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SPHERE_0);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SPHERE_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SPHERE_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SPHERE_1);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SPHERE_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SPHERE_2, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SPHERE_2);
		break;
	}

}

void CSkill_SphereState::Exit(CRui* pRui)
{
	if(m_eStateType != CRuiState::TYPE_START)
		m_pCollBox->Set_Dead();
}


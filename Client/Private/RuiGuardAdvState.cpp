#include "stdafx.h"
#include "RuiGuardAdvState.h"
#include "RuiIdleState.h"
#include "RuiGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Rui;


CGuardAdvState::CGuardAdvState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiState * CGuardAdvState::HandleInput(CRui* pRui)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pRui->Get_Model()->Get_End(CRui::ANIM_GUARD_ADV))
	{
		pRui->Get_Model()->Set_End(CRui::ANIM_GUARD_ADV);

		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_O))
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_C))
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
			break;
		default:
			break;
		}
	
	}


	return nullptr;
}

CRuiState * CGuardAdvState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_bGuard(true);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_GUARD_ADV);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_GUARD_ADV, 0.02f);


	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CRuiState * CGuardAdvState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	_vector vCollPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
	_vector vCollLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
	vCollPos += XMVector3Normalize(vCollLook) * 1.5f; //추가
	vCollPos.m128_f32[1] = 1.f; //추가
	m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
	CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	m_fMove += fTimeDelta;
	if (m_fMove > 0.1f && !m_bHit)
	{
		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			
			//pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, vTagetWorld);

			RELEASE_INSTANCE(CEffect_Manager);

			m_bHit = true;
		}

	}
	if (m_fMove < 0.3f && m_bHit)
	{
		_vector vTagetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vTagetPos += XMVector3Normalize(vPos) * 30.f * fTimeDelta;
		vTagetPos.m128_f32[1] = 0.f;
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTagetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTagetPos);
	}

	pRui->Get_Model()->Play_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardAdvState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_GUARD_ADV);
	pRui->Set_AnimIndex(CRui::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CRui* pRui)
{
	m_pCollBox->Set_Dead(); //추가
}


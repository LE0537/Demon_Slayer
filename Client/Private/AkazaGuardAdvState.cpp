#include "stdafx.h"
#include "AkazaGuardAdvState.h"
#include "AkazaIdleState.h"
#include "AkazaGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CGuardAdvState::CGuardAdvState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CAkazaState * CGuardAdvState::HandleInput(CAkaza* pAkaza)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_GUARD_ADV))
	{
		pAkaza->Get_Model()->Set_End(CAkaza::ANIM_GUARD_ADV);

		switch (pAkaza->Get_i1P())
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

CAkazaState * CGuardAdvState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_bGuard(true);
	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_GUARD_ADV);
	pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_GUARD_ADV, 0.02f);


	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CAkazaState * CGuardAdvState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

	_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
	_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
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
		_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vTagetPos += XMVector3Normalize(vPos) * 30.f * fTimeDelta;
		vTagetPos.m128_f32[1] = 0.f;
		m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTagetPos);
	}

	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardAdvState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_GUARD_ADV);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead(); //추가
}


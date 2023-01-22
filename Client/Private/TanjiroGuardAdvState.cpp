#include "stdafx.h"
#include "TanjiroGuardAdvState.h"
#include "TanjiroIdleState.h"
#include "TanjiroGuardState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"

using namespace Tanjiro;


CGuardAdvState::CGuardAdvState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CGuardAdvState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_GUARD_ADV))
		{
			pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_GUARD_ADV);

			if (pGameInstance->Key_Pressing(DIK_O))
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
		}
		break;
	case 2:
		if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_GUARD_ADV))
		{
			pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_GUARD_ADV);

			if (pGameInstance->Key_Pressing(DIK_C))
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
		}
		break;
	default:
		break;
	}

	return nullptr;
}

CTanjiroState * CGuardAdvState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_bGuard(true);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_ADV);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_ADV, 0.02f);


	printf_s("type : %d state : %d anim : %d \n", (int)m_eStateType, (int)m_eStateId, (int)pTanjiro->Get_AnimIndex());

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CTanjiroState * CGuardAdvState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pTanjiro->Get_Transform()->LookAt(vLooAt);

	_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
	_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
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

			_float4 vTagetPos2;
			XMStoreFloat4(&vTagetPos2, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			vTagetPos2.y += 2.f;
			pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos2);

			RELEASE_INSTANCE(CEffect_Manager);

			m_bHit = true;
		}

	}
	if (m_fMove < 0.3f && m_bHit)
	{
		_vector vTagetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vTagetPos += XMVector3Normalize(vPos) * 30.f * fTimeDelta;
		vTagetPos.m128_f32[1] = 0.f;
		m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTagetPos);
	}

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardAdvState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_ADV);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead(); //추가
}


#include "stdafx.h"
#include "KyoujuroGuardAdvState.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroGuardState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Tanjiro.h"

using namespace Kyoujuro;


CGuardAdvState::CGuardAdvState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CGuardAdvState::HandleInput(CKyoujuro* pKyoujuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_GUARD_ADV))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_GUARD_ADV);

		if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
	}


	return nullptr;
}

CKyoujuroState * CGuardAdvState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_bGuard(true);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_ADV);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_GUARD_ADV, 0.02f);


	printf_s("type : %d state : %d anim : %d \n", (int)m_eStateType, (int)m_eStateId, (int)pKyoujuro->Get_AnimIndex());

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CKyoujuroState * CGuardAdvState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pKyoujuro->Get_Transform()->LookAt(vLooAt);

	_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
	_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
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
		_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vTagetPos += XMVector3Normalize(vPos) * 30.f * fTimeDelta;
		vTagetPos.m128_f32[1] = 0.f;
		m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTagetPos);
	}

	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardAdvState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_GUARD_ADV);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CKyoujuro* pKyoujuro)
{
	m_pCollBox->Set_Dead(); //추가
}


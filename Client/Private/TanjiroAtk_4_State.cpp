#include "stdafx.h"
#include "TanjiroAtk_4_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CAtk_4_State::CAtk_4_State()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CAtk_4_State::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	return nullptr;
}

CTanjiroState * CAtk_4_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_4);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_4, 0.01f);


	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_4))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_4);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_4_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pTanjiro->Get_Transform()->LookAt(vLooAt);

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.3f)
	{
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.5f);
		CCollider*	pMyCollider = pTanjiro->Get_Collider();
		CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), TEXT("Com_SPHERE"));

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}
	else if (m_fMove > 0.15f)
	{
		if (!m_bHit)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pTanjiro->Get_PlayerInfo().iDmg * 3);
				dynamic_cast<CKyoujuro*>(m_pTarget)->Take_Damage(0.5f);

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				vTagetPos.y += 2.f;
				pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CAtk_4_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_4;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_4);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_4);

}

void CAtk_4_State::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead(); //추가
}


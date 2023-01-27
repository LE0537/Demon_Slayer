#include "stdafx.h"
#include "TanjiroAtk_3_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroAtk_4_State.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"

using namespace Tanjiro;


CAtk_3_State::CAtk_3_State()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CAtk_3_State::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	
	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J) && m_fComboDelay <= 43.f)
			m_bAtkCombo = true;
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z) && m_fComboDelay <= 43.f)
			m_bAtkCombo = true;
		break;
	default:
		break;
	}


	return nullptr;
}

CTanjiroState * CAtk_3_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_3);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_3, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);



	if (m_bAtkCombo == true && m_fTime >= 40.f)
		return new CAtk_4_State();

	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_3))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_3);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_3_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pTanjiro->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.5f)
	{
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);

		CCollider*	pMyCollider = pTanjiro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

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
	else if (m_fMove < 0.65f)
	{
		if (!m_bHit)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-pTanjiro->Get_PlayerInfo().iDmg * 2);
					m_pTarget->Take_Damage(0.6f,false);
				}

				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, vTagetWorld);

				RELEASE_INSTANCE(CEffect_Manager);
			
				m_bHit = true;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);
	if (!m_bEffect)
	{
		_matrix vPlayerWorld = pTanjiro->Get_Transform()->Get_WorldMatrix();

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK3, vPlayerWorld);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAtk_3_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_3;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_3);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_3);

}

void CAtk_3_State::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead(); //추가
}


#include "stdafx.h"
#include "KyoujuroAtk_3_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_4_State.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"

using namespace Kyoujuro;


CAtk_3_State::CAtk_3_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CAtk_3_State::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
			m_bAtkCombo = true;
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
			m_bAtkCombo = true;
		break;
	default:
		break;
	}

	return nullptr;
}

CKyoujuroState * CAtk_3_State::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_ATTACK_3);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_ATTACK_3, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);


	if (m_bAtkCombo == true && m_fTime >= 60.f)
		return new CAtk_4_State();



	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_ATTACK_3))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_ATTACK_3);
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CAtk_3_State::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pKyoujuro->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.7f)
	{
		pKyoujuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);

		if (m_iHit < 1)
		{
			_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 1.f; //추가
			vCollPos.m128_f32[1] = 0.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-pKyoujuro->Get_PlayerInfo().iDmg);
					m_pTarget->Take_Damage(0.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				vTagetPos.y += 2.f;
				pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos);

				RELEASE_INSTANCE(CEffect_Manager);

				++m_iHit;
			}
		}


		CCollider*	pMyCollider = pKyoujuro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pKyoujuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}
	else if(m_fMove < 1.f)
	{
		if (!m_bHit)
		{
			_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-pKyoujuro->Get_PlayerInfo().iDmg * 2);
					m_pTarget->Take_Damage(0.6f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				vTagetPos.y += 2.f;
				pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	return nullptr;
}

void CAtk_3_State::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_ATK_3;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_ATTACK_3);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_ATTACK_3);

}

void CAtk_3_State::Exit(CKyoujuro * pKyoujuro)
{
	m_pCollBox->Set_Dead(); //추가
}


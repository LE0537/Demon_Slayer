#include "stdafx.h"
#include "KyoujuroAtk_4_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CAtk_4_State::CAtk_4_State()
{
}

CKyoujuroState * CAtk_4_State::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CKyoujuroState * CAtk_4_State::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_ATTACK_4);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_ATTACK_4, 0.01f);


	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_ATTACK_4))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_ATTACK_4);
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CAtk_4_State::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"))->Get_LayerFront();
	pKyoujuro->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.3f)
	{
		pKyoujuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.5f);
		CCollider*	pMyCollider = pKyoujuro->Get_Collider();
		CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

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

			pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}
	if (!m_bHit)
	{
		CCollider*	pMyCollider = dynamic_cast<CKyoujuroWeapon*>(pKyoujuro->Get_Weapon())->Get_WeaponColl();
		CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float4 vTagetPos;
			XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			m_pTarget->Get_Transform()->LookAt(vPos);
			m_pTarget->Set_Hp(-pKyoujuro->Get_PlayerInfo().iDmg * 3);
			dynamic_cast<CTanjiro*>(m_pTarget)->Take_Damage(0.5f);

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			vTagetPos.y += 2.f;
			pEffectManger->Create_Effect(CEffect_Manager::EFFECT_HIT, vTagetPos);

			RELEASE_INSTANCE(CEffect_Manager);

			m_bHit = true;
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CAtk_4_State::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_ATK_4;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_ATTACK_4);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_ATTACK_4);

}

void CAtk_4_State::Exit(CKyoujuro * pKyoujuro)
{
}


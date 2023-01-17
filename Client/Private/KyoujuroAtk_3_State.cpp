#include "stdafx.h"
#include "KyoujuroAtk_3_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_4_State.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"

using namespace Kyoujuro;


CAtk_3_State::CAtk_3_State()
{
}

CKyoujuroState * CAtk_3_State::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		m_bAtkCombo = true;

	//if (pGameInstance->Key_Down(DIK_J) && m_fComboDelay <= 43.f)
	//	m_bAtkCombo = true;

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

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"))->Get_LayerFront();
	pKyoujuro->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.5f)
	{
		pKyoujuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);

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
	else
	{
		if (!m_bHit)
		{
			CCollider*	pMyCollider = dynamic_cast<CKyoujuroWeapon*>(pKyoujuro->Get_Weapon())->Get_WeaponColl();
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pKyoujuro->Get_PlayerInfo().iDmg * 3.f);

				m_bHit = true;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

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
}


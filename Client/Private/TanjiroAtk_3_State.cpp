#include "stdafx.h"
#include "TanjiroAtk_3_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroAtk_4_State.h"
#include "TanjiroWeapon.h"
#include "Layer.h""
using namespace Tanjiro;


CAtk_3_State::CAtk_3_State()
{
}

CTanjiroState * CAtk_3_State::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	//if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
	//	m_bAtkCombo = true;

	if (pGameInstance->Key_Down(DIK_J))
		m_bAtkCombo = true;


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

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront();
	pTanjiro->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.5f)
	{
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);

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

			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}
	else
	{
		if (!m_bHit)
		{
			CCollider*	pMyCollider = dynamic_cast<CTanjiroWeapon*>(pTanjiro->Get_Weapon())->Get_WeaponColl();
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pTanjiro->Get_PlayerInfo().iDmg * 3.f);

				m_bHit = true;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

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
}


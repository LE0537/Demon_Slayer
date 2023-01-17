#include "stdafx.h"
#include "TanjiroAtk_1_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroAtk_2_State.h"
#include "TanjiroWeapon.h"
#include "Layer.h""
using namespace Tanjiro;


CAtk_1_State::CAtk_1_State()
{
}

CTanjiroState * CAtk_1_State::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	//if (pGameInstance->Mouse_Down(DIMK_LBUTTON) && m_fComboDelay <=43.f)
	//	m_bAtkCombo = true;

	if (pGameInstance->Key_Down(DIK_J) && m_fComboDelay <= 43.f)
		m_bAtkCombo = true;

	return nullptr;
}

CTanjiroState * CAtk_1_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_1);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_1, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);


	if (m_bAtkCombo == true && m_fTime >= 33.f)
		return new CAtk_2_State();



	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_1))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_1);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_1_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront();
	pTanjiro->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.3f)
	{
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.3f);

		CCollider*	pMyCollider = dynamic_cast<CTanjiroWeapon*>(pTanjiro->Get_Weapon())->Get_WeaponColl();
		CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), TEXT("Com_SPHERE"));
		CCollider*	pMyCollider2 = pTanjiro->Get_Collider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{

				//_vector vTagetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pTanjiro->Get_PlayerInfo().iDmg);

				m_bHit = true;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
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

	RELEASE_INSTANCE(CGameInstance);


	return nullptr;
}

void CAtk_1_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_1;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_1);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_1);

}

void CAtk_1_State::Exit(CTanjiro * pTanjiro)
{
}


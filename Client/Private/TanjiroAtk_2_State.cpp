#include "stdafx.h"
#include "TanjiroAtk_2_State.h"
#include "TanjiroIdleState.h"
#include "TanjiroAtk_3_State.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h""
using namespace Tanjiro;


CAtk_2_State::CAtk_2_State()
{
}

CTanjiroState * CAtk_2_State::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	//if (pGameInstance->Mouse_Down(DIMK_LBUTTON) && m_fComboDelay <= 43.f)
	//	m_bAtkCombo = true;

	if (pGameInstance->Key_Down(DIK_J) && m_fComboDelay <= 43.f)
		m_bAtkCombo = true;

	return nullptr;
}

CTanjiroState * CAtk_2_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_2);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_2, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);


	if (m_bAtkCombo == true && m_fTime >= 30.f)
		return new CAtk_3_State();



	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_2))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_2);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_2_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))->Get_LayerFront();
	pTanjiro->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 0.7f);
	m_fMove += fTimeDelta;
	if (m_fMove < 0.3f)
	{
		if (m_iHit < 1)
		{

			CCollider*	pMyCollider = dynamic_cast<CTanjiroWeapon*>(pTanjiro->Get_Weapon())->Get_WeaponColl();
			CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), TEXT("Com_SPHERE"));

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->LookAt(vPos);
				m_pTarget->Set_Hp(-pTanjiro->Get_PlayerInfo().iDmg);

				++m_iHit;
			}
		}
	}
	else if (m_fMove >= 0.3f)
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
				m_pTarget->Set_Hp(-pTanjiro->Get_PlayerInfo().iDmg);

				m_bHit = true;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);


	return nullptr;
}

void CAtk_2_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_2;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_2);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_2);

}

void CAtk_2_State::Exit(CTanjiro * pTanjiro)
{
}


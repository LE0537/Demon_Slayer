#include "stdafx.h"
#include "KyoujuroAtk_3_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_4_State.h"


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
}


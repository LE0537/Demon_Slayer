#include "stdafx.h"
#include "TanjiroAtk_1_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroAtk_2_State.h"

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
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);




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


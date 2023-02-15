#include "stdafx.h"
#include "RuiDadAtk_1_State.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDadDashState.h"


using namespace RuiDad;


CAtk_1_State::CAtk_1_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState * CAtk_1_State::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CRuiDadState * CAtk_1_State::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{


	if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_ATTACK_1))
	{
		pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_ATTACK_1);
		return new CIdleState();
	}





	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_1);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_1, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;


	//if (m_bAtkCombo == true && m_fTime >= 40.f)
	//	return new CAtk_2_State();


	if (m_bAtkCombo == false)
	{
		if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_ATTACK_1))
		{
			pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_ATTACK_1);
			return new CIdleState();
		}
	}


	return nullptr;
}

CRuiDadState * CAtk_1_State::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	return nullptr;
}

void CAtk_1_State::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_ATK_1;
	pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_1);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_1);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_1, false);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_1, 0.01f);
}

void CAtk_1_State::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead(); //Ãß°¡
	pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
}


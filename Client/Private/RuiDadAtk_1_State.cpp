#include "stdafx.h"
#include "RuiDadAtk_1_State.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDadDashState.h"


using namespace RuiDad;


CAtk_1_State::CAtk_1_State(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	m_eStateType = eType;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState * CAtk_1_State::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CRuiDadState * CAtk_1_State::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{


	
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START: // 1타
			if(m_bNextAnim)
				return new CAtk_1_State(TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP: // 2타
			if (m_bNextAnim)
				return new CAtk_1_State(TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END: // 3타
			if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIMID::ANIM_ATTACK_2))
			{
				pRuiDad->Get_Model()->Set_End(CRuiDad::ANIMID::ANIM_ATTACK_2);
				return new CIdleState();
			}
			break;
		}


	m_fDuration = pRuiDad->Get_Model()->Get_Duration_Index(pRuiDad->Get_AnimIndex());
	m_fCurrentDuration = pRuiDad->Get_Model()->Get_CurrentTime_Index(pRuiDad->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;


	if (fRatio >= 0.7f)
		m_bNextAnim = true;


	////////////////////////////// 콜박스 여기서 하면될듯

	//switch (m_eStateType)
	//{
	//case Client::CRuiDadState::TYPE_START: // 1타

	//	break;
	//case Client::CRuiDadState::TYPE_LOOP: // 2타
	//
	//	break;
	//case Client::CRuiDadState::TYPE_END: // 3타

	//	break;
	//}
	//////////////////////////////


	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;


	//if (m_bAtkCombo == true && m_fTime >= 40.f)
	//	return new CAtk_2_State();


	//if (m_bAtkCombo == false)
	//{
	//	if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_ATTACK_1))
	//	{
	//		pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_ATTACK_1);
	//		return new CIdleState();
	//	}
	//}


	return nullptr;
}

CRuiDadState * CAtk_1_State::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	

	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CAtk_1_State::Enter(CRuiDad* pRuiDad)
{
	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		m_eStateId = STATE_ID::STATE_ATK_1;
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_0);
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_0, false);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_0, 0.01f);
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		m_eStateId = STATE_ID::STATE_ATK_2;
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_1);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_1);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_1, false);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_1, 0.01f);
		break;
	case Client::CRuiDadState::TYPE_END:
		m_eStateId = STATE_ID::STATE_ATK_3;
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_2);
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_2);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_2);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_2, false);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_2, 0.01f);
		break;

	default:
		break;
	}



}

void CAtk_1_State::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead(); //추가
	//pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
}


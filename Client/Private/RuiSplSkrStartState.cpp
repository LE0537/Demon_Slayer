#include "stdafx.h"
#include "..\Public\RuiSplSkrStartState.h"
#include "GameInstance.h"
#include "Rui_CinemaState.h"
#include "RuiIdleState.h"
#include "RuiSplColl.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Rui;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CSplSkrStartState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CSplSkrStartState::Tick(CRui* pRui, _float fTimeDelta)
{
	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
		/*	if (pRui->Get_HitSpl())
			{
				pRui->Set_HitSpl(false);
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				pRui->Get_BattleTarget()->Play_Scene();
				return new CRui_CinemaState(CRui_CinemaState::SCENE_START);
			}
			else*/
			{
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CRuiState::TYPE_LOOP:

			break;
		case Client::CRuiState::TYPE_END:
			break;
		case Client::CRuiState::TYPE_DEFAULT:
			break;
		case Client::CRuiState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		//if (m_bCollision == true && m_bCreate == false)
		//{
		//	m_bPlayScene = true;
		//	g_bSpecialSkillHit = true;
		//	m_bCreate = true;
		//	pTanjiro->Get_BattleTarget()->Take_Damage(0.f, false);
		//	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		//	dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 1.f);
		//}

		//if (m_bPlayScene == true)
		//{
		//	if (g_bSpecialSkillHit == false)
		//	{
		//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		//		pTanjiro->Get_BattleTarget()->Play_Scene();
		//		return new CHinoCami_CinemaState(CHinoCami_CinemaState::SCENE_START);
		//	}
		//}

		if (pRui->Get_HitSpl())
		{
			pRui->Set_HitSpl(false);
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			m_bPlayScene = true;
			g_bSpecialSkillHit = true;
			pRui->Get_BattleTarget()->Take_Damage(0.f, false);
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 1.f);
		}
	

		if (m_bPlayScene == true)
		{
			if (g_bSpecialSkillHit == false)
			{
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				pRui->Get_BattleTarget()->Play_Scene();
				return new CRui_CinemaState(CRui_CinemaState::SCENE_START);
			}
		}

	

		break;
	case Client::CRuiState::TYPE_LOOP:
		break;
	case Client::CRuiState::TYPE_END:
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CRuiState * CSplSkrStartState::Late_Tick(CRui* pRui, _float fTimeDelta)
{


	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pRui->Get_Transform()->Set_PlayerLookAt(vLooAt);

	if (m_eStateType == CRuiState::TYPE_START)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		CRuiSplColl::RUISPLINFO	tInfo;
		tInfo.pPlayer = pRui;
		tInfo.pTarget = m_pTarget;

		if (m_iHit < 1)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			tInfo.iIndex = 0;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiSplColl"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;


			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}

		RELEASE_INSTANCE(CGameInstance);
	}

	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_SPLSKL_START_0);
		pRui->Set_AnimIndex(CRui::ANIM_SPLSKL_START_0);
		pRui->Get_Model()->Set_Loop(CRui::ANIM_SPLSKL_START_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SPLSKL_START_0, 0.01f);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_SplSkr_Start.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SplSkr_Start.wav"), g_fEffect);
		break;
	case Client::CRuiState::TYPE_END:
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CRui* pRui)
{
}

void CSplSkrStartState::Move(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}

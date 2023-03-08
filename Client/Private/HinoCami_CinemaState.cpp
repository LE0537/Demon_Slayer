#include "stdafx.h"
#include "..\Public\HinoCami_CinemaState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "ImGuiManager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"
#include "BattleDialog.h"

using namespace Tanjiro;

CHinoCami_CinemaState::CHinoCami_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;

}

CTanjiroState * CHinoCami_CinemaState::HandleInput(CTanjiro * pTanjiro)
{
	CTanjiroState* pState = nullptr;
	//if (pGameInstance->Key_Down(DIK_F5) && !pTanjiro->Get_StoryKey())
	//	pState = new CHinoCami_CinemaState(CHinoCami_CinemaState::CINEMASCENE::SCENE_START);
	//if (pGameInstance->Key_Down(DIK_F6) && !pTanjiro->Get_StoryKey())
	//	pState = new CHinoCami_CinemaState(CHinoCami_CinemaState::CINEMASCENE::SCENE_0);
	//if (pGameInstance->Key_Down(DIK_F7) && !pTanjiro->Get_StoryKey())
	//	pState = new CHinoCami_CinemaState(CHinoCami_CinemaState::CINEMASCENE::SCENE_1);
	//if (pGameInstance->Key_Down(DIK_F8) && !pTanjiro->Get_StoryKey())
	//	pState = new CHinoCami_CinemaState(CHinoCami_CinemaState::CINEMASCENE::SCENE_2);
	//if (pGameInstance->Key_Down(DIK_F9) && !pTanjiro->Get_StoryKey())
	//	pState = new CHinoCami_CinemaState(CHinoCami_CinemaState::CINEMASCENE::SCENE_3);


	if(nullptr != pState)
		pTanjiro->Get_BattleTarget()->Play_Scene();

	return pState;
}

CTanjiroState * CHinoCami_CinemaState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_START:
		if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_START))
		{
			pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_START);
			return new CHinoCami_CinemaState(SCENE_0);
		}
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
		if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_0))
		{
			pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_0);
			return new CHinoCami_CinemaState(SCENE_1);
		}
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1:
		if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_1))
		{
			pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_1);

			return new CHinoCami_CinemaState(SCENE_2);
		}
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2:
		if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_2))
		{
			pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_2);

			return new CHinoCami_CinemaState(SCENE_3);
		}
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3:
		if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_3))
		{
			pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_3);
			if (pTanjiro->Get_StorySplEnd())
			{
				CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

				pUIManager->Set_StroyEventEnd(true);

				RELEASE_INSTANCE(CUI_Manager);
			}
			else
			{
	/*			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_FADE, this);
				
				RELEASE_INSTANCE(CEffect_Manager);*/
			}

			
			return new CHinoCami_CinemaState(SCENE_4);
		}
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_4:
		if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_SPLSKL_END))
		{
			pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_SPLSKL_END);

			return new CIdleState();
		}
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_5:
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

	if (m_eScene == CHinoCami_CinemaState::SCENE_4)
	{
		if(m_bNextAnim == false)
			Fall_Height(pTanjiro, fTimeDelta);
	}
	



	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{


	pTanjiro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHinoCami_CinemaState::Enter(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = nullptr;
	CEffect_Manager* pEffectManger = nullptr;
	CUI_Manager* pUI_Manager = nullptr;
	switch (m_eScene)
	{
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_START:
		pTanjiro->Set_SplSkl(true);
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pTanjiro->Get_NavigationHeight().y, 64.f, 1.f));
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.2f, pTanjiro->Get_NavigationHeight().y, 64.f, 1.f));
		pTanjiro->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pTanjiro->Get_NavigationHeight().y, 64.f, 1.f));
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pTanjiro->Get_NavigationHeight().y, 64.f, 1.f));
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_START, 0.01f);
		pGameInstance = GET_INSTANCE(CGameInstance);
		((CCamera_Dynamic*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront()))->Start_CutScene(true, CCamera_Dynamic::CUTSCENE_TAN_SPC_1);
		RELEASE_INSTANCE(CGameInstance);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_SplSkr.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_SE_SplSkr.wav"), g_fEffect);
		pUI_Manager = GET_INSTANCE(CUI_Manager);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplCharNum(0);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplDialogStart(true);
		RELEASE_INSTANCE(CUI_Manager);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0: {
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_0, 0.01f);

		pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_BOOM1, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_BOOM2, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_BOOM3, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_BOOM4, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_BOOM5, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_BOOM6, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SLASH1, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SLASH2, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SLASH3, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SLASH4, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SLASH5, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SLASH6, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_PROJ1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_PROJ2, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_PROJ3, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_PROJ4, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_PROJ5, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO2_SWORD, pTanjiro->Get_WeaponWorld());
		
		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1: {
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_1, 0.01f);

		pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO3_PROJ1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO3_SLASH1, pTanjiro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO3_SLASH2, pTanjiro->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);

		pUI_Manager = GET_INSTANCE(CUI_Manager);
		if(pUI_Manager->Get_BattleTypeCheck())
			pUI_Manager->Set_MsgCount(1);
		RELEASE_INSTANCE(CUI_Manager);
		break;
	}
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2: {
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_2, 0.01f);

		pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO4_PROJ1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO4_SLASH1, pTanjiro->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3: {
		pTanjiro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_3, 0.01f);

		pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO5_PROJ1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO5_PROJ2, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO5_SLASH1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO5_SLASH2, pTanjiro);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO5_SLASH3, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_MO5_GOUND, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_4:
		pTanjiro->Get_BattleTarget()->Set_Hp(-400);
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, 10.f, 56.1f, 1.f));
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pTanjiro->Get_NavigationHeight().y, 50.03f, 1.f));

		pTanjiro->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_BOUND, 20.f, 30.f, 0.f);
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SPLSKL_END);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SPLSKL_END);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_SPLSKL_END);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SPLSKL_END, 0.2f);

		pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_ENDGROUND, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_HINO_ENDPLAYER, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_5:
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CHinoCami_CinemaState::Exit(CTanjiro * pTanjiro)
{

}

void CHinoCami_CinemaState::Fall_Height(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);
	static _float fGravity = -80.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	fVelocity += fGravity * fTimeDelta;

	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	if (vPosition.y <= m_fOriginPosY)// m_fCurrentPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;
		pTanjiro->Get_Transform()->Set_Jump(false);
		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		vecPos = XMVectorSetY(vecPos, vPosition.y);
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
	{
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}



}


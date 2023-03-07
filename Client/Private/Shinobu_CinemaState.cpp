#include "stdafx.h"
#include "..\Public\Shinobu_CinemaState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "BattleDialog.h"

using namespace Shinobu;

CShinobu_CinemaState::CShinobu_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CShinobu_CinemaState::HandleInput(CShinobu * pShinobu)
{
	CShinobuState* pState = nullptr;
	
	return pState;
}

CShinobuState * CShinobu_CinemaState::Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_START))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_START);
			return new CShinobu_CinemaState(SCENE_0);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_0))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_0);
			return new CShinobu_CinemaState(SCENE_1);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_1))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_1);
			return new CShinobu_CinemaState(SCENE_2);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_2))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_2);
			return new CShinobu_CinemaState(SCENE_3);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_3))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_3);
			return new CShinobu_CinemaState(SCENE_END);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_END:
		if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_SPLSKL_END))
		{
			pShinobu->Get_Model()->Set_End(CShinobu::ANIM_SPLSKL_END);
			return new CIdleState();
		}
		break;
	default:
		break;
	}


	if (m_eScene == SCENE_END && m_bNextAnim == false)
		Jump(pShinobu, fTimeDelta);




	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Late_Tick(CShinobu * pShinobu, _float fTimeDelta)
{

	pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CShinobu_CinemaState::Enter(CShinobu * pShinobu)
{
	CGameInstance* pGameInstance = nullptr;
	CUI_Manager* pUI_Manager = nullptr;
	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(62.66f, pShinobu->Get_NavigationHeight().y, 47.29f, 1.f));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(62.86f, pShinobu->Get_NavigationHeight().y, 47.29f, 1.f));
		pShinobu->Get_Transform()->Set_PlayerLookAt(XMVectorSet(56.46f, pShinobu->Get_NavigationHeight().y, 56.46f, 1.f));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(56.46f, pShinobu->Get_NavigationHeight().y, 56.46f, 1.f));
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_START));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_START, 0.01f);
		pShinobu->Set_SplSkl(true);

		pGameInstance = GET_INSTANCE(CGameInstance);
		((CCamera_Dynamic*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront()))->Start_CutScene(true, CCamera_Dynamic::CUTSCENE_SNB_START);
		RELEASE_INSTANCE(CGameInstance);
		pUI_Manager = GET_INSTANCE(CUI_Manager);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplCharNum(5);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplDialogStart(true);
		RELEASE_INSTANCE(CUI_Manager);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Shinobu_SplSkr.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_SplSkr.wav"), g_fEffect);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_0);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_0);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_0));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_0, 0.01f);
		pShinobu->Get_BattleTarget()->Set_SceneRender(false);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_1);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_1);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_1));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_1, 0.01f);
		pShinobu->Get_BattleTarget()->Set_SceneRender(true);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_2);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_2);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_2));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		pShinobu->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_3);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_3);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_3));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_3);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_END:
		pShinobu->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_BOUND, 20.f, 30.f, 3.f);
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SPLSKL_END);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SPLSKL_END);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu::ANIM_SPLSKL_END));
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SPLSKL_END);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SPLSKL_END, 0.01f);


		m_fBoundPower = 20.f;
		m_fJumpPower = 30.f;
		m_fKnockBackPower = 8.f;
		break;
	default:
		break;
	}


}

void CShinobu_CinemaState::Exit(CShinobu * pShinobu)
{
}

void CShinobu_CinemaState::Jump(CShinobu * pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 5.f;

	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	m_vVelocity.y += 50.f * fTimeDelta;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;



	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;

	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

	//m_vPosition.y -= 3.f;



	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPosition))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pShinobu->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}
}


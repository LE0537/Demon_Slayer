#include "stdafx.h"
#include "RuiJumpSkill_Common.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiBigBall.h"

using namespace Rui;


CJumpSkill_CommonState::CJumpSkill_CommonState(STATE_TYPE eType)
{

	m_eStateType = eType;

}

CRuiState * CJumpSkill_CommonState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CRuiState * CJumpSkill_CommonState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_EffectTime(fTimeDelta);
	m_fDelay += fTimeDelta;

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CJumpSkill_CommonState(CRuiState::TYPE_DEFAULT);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			//pRui->Get_Transform()->LookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

			return new CJumpSkill_CommonState(CRuiState::TYPE_DEFAULT);
			break;
		case Client::CRuiState::TYPE_DEFAULT:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			
			break;
		case Client::CRuiState::TYPE_CHANGE:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
	}

	//if (m_eStateType == TYPE_START)
	//{
	//	if(m_fDelay >= 0.5f)
	//		return new CJumpSkill_CommonState(CRuiState::TYPE_LOOP);
	//}


	if (m_eStateType == TYPE_DEFAULT)
		Jump(pRui, fTimeDelta);

	if (m_bNextAnim == true)
		return new CJumpSkill_CommonState(CRuiState::TYPE_CHANGE);

	return nullptr;
}

CRuiState * CJumpSkill_CommonState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);
	if(m_eStateType == CRuiState::TYPE_START)
		m_fMove += fTimeDelta;

	CRuiBigBall::RUIBIGBALLINFO	tInfo;
	tInfo.pPlayer = pRui;
	tInfo.pTarget = m_pTarget;

	if (m_fMove > 0.17f && m_iHit < 1 && pRui->Get_BattleTarget()->Get_GodMode() == false)
	{
		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiBigBall"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
			return nullptr;

		RELEASE_INSTANCE(CGameInstance);
		++m_iHit;
	}

	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.3f);
	if (!m_bEffect && m_eStateType == CRuiState::TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_JUMPBALL_START, pRui);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CJumpSkill_CommonState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_JUMP_ATTACK;


	_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMPSKILL_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMPSKILL_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMPSKILL_0);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		//vPosition += XMVectorSet(0.3f, 0.3f, 0.3f, 0.f);
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		pRui->Get_Transform()->LookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Commonn.wav"), fVOICE);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_CommonSkill.wav"), fEFFECT);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMPSKILL_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMPSKILL_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMPSKILL_1);
		pRui->Get_Model()->Reset_Anim(CRui::ANIM_JUMPSKILL_0);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		//vPosition += XMVectorSet(0.3f, 0.3f, 0.3f, 0.f);
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		pRui->Get_Transform()->LookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_LOOP_END);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_LOOP_END, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_LOOP_END);
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex(), true);
		Initialize_value(pRui);
		break;
	case Client::CRuiState::TYPE_CHANGE:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_END);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_END, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_END);
		pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_LOOP_END);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		break;
	}

	if (pRui->Get_BattleTarget()->Get_TargetState() == STATE_HIT)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_Hit_CommonSkill.wav"), fEFFECT);

}

void CJumpSkill_CommonState::Exit(CRui* pRui)
{

}

void CJumpSkill_CommonState::Jump(CRui * pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pRui->Get_NavigationHeight().y;
	pRui->Get_Transform()->Set_Jump(true);

	static _float fGravity = -150.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	
	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		pRui->Get_Transform()->Set_Jump(false);
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pRui->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
			m_bNextAnim = true;
		}
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vecPos))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpSkill_CommonState::Initialize_value(CRui * pRui)
{
	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;



}


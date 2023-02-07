#include "stdafx.h"
#include "RuiJumpSkill_Move.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiShoot.h"


using namespace Rui;


CJumpSkill_MoveState::CJumpSkill_MoveState(STATE_TYPE eType)
{

	m_eStateType = eType;

}

CRuiState * CJumpSkill_MoveState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CRuiState * CJumpSkill_MoveState::Tick(CRui* pRui, _float fTimeDelta)
{
	m_fDelay += fTimeDelta;

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CJumpSkill_MoveState(CRuiState::TYPE_DEFAULT);
			break;
		case Client::CRuiState::TYPE_DEFAULT:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			/*if(m_bNextAnim == true)
				return new CJumpSkill_MoveState(CRuiState::TYPE_CHANGE);*/
			break;
		case Client::CRuiState::TYPE_CHANGE:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
	}



	if (m_eStateType == TYPE_DEFAULT)
		Jump(pRui, fTimeDelta);


	if (m_bNextAnim == true)
		return new CJumpSkill_MoveState(CRuiState::TYPE_CHANGE);

	return nullptr;
}

CRuiState * CJumpSkill_MoveState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	if (m_eStateType == CRuiState::TYPE_START)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		CRuiShoot::RUISHOOTINFO	tInfo;
		tInfo.pPlayer = pRui;
		tInfo.pTarget = m_pTarget;

		if (m_iHit < 1)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			tInfo.iIndex = 3;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;
			tInfo.iIndex = 4;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;
			tInfo.iIndex = 5;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}

		RELEASE_INSTANCE(CGameInstance);
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.3f);
	if (!m_bEffect && m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_JUMPSHOTNET_START, pRui);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CJumpSkill_MoveState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_JUMP;


	_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_MOVESKILL);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_MOVESKILL, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_MOVESKILL);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		//vPosition += XMVectorSet(0.3f, 0.3f, 0.3f, 0.f);
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		pRui->Get_Transform()->LookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
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

}

void CJumpSkill_MoveState::Exit(CRui* pRui)
{

}

void CJumpSkill_MoveState::Jump(CRui * pRui, _float fTimeDelta)
{
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

void CJumpSkill_MoveState::Initialize_value(CRui * pRui)
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


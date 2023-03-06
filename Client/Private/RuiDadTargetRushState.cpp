#include "stdafx.h"
#include "RuiDadTargetRushState.h"
#include "GameInstance.h"
#include "RuiDadIdleState.h"
#include "RuiDadMoveState.h"
#include "RuiDadDashState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace RuiDad;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiDadState * CTargetRushState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CRuiDadState * CTargetRushState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{

	pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START: 
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP: 
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());;
			break;
		case Client::CRuiDadState::TYPE_END: 
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_LOOP:
		Move(pRuiDad, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CRuiDadState * CTargetRushState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pRuiDad->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRuiDad->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_JUMP_ATTACK;

	_uint iRand = rand() % 2;

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_RUSH_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_RUSH_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_RUSH_0);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_RUSH_0, 0.01f);
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_RUSH_1);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_RUSH_1);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_RUSH_1, true);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiDad_SE_RushSkill.wav"), g_fEffect);	
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_RushSkill_0.wav"), g_fVoice);
		else
			CSoundMgr::Get_Instance()->PlayVoice(TEXT("RuiDad_RushSkill_1.wav"), g_fVoice);
		Initialize_value(pRuiDad);
		break;
	case Client::CRuiDadState::TYPE_END:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_RUSH_2);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_RUSH_2);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_RUSH_2, false);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIMID::ANIM_RUSH_2);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIMID::ANIM_RUSH_2, 0.01f);
		break;
	}


}

void CTargetRushState::Exit(CRuiDad* pRuiDad)
{
}

void CTargetRushState::Move(CRuiDad* pRuiDad, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	vMyPosition = XMVectorSetY(vMyPosition, pRuiDad->Get_NavigationHeight().y);
	vTargetPosition = XMVectorSetY(vTargetPosition, pRuiDad->Get_BattleTarget()->Get_NavigationHeight().y);


	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	if (pRuiDad->Get_BattleTarget()->Get_Transform()->Get_Jump())
	{
		if (fDistance <= 5.f)
		{
			m_bNextAnim = true;
		}
	}

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}


	if (pRuiDad->Get_SphereCollider()->Collision(pRuiDad->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pRuiDad->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pRuiDad->Get_BattleTarget()->Get_PlayerInfo().bGuard && pRuiDad->Get_BattleTarget()->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pRuiDad->Get_BattleTarget()->Get_GuardHit(0);
		}
		else 
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			RELEASE_INSTANCE(CGameInstance);
			pRuiDad->Get_BattleTarget()->Take_Damage(0.3f, false);
		}
	}
	else
		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CRuiDad* pRuiDad)
{
	m_vPosition.x = XMVectorGetX(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRuiDad->Set_NavigationHeight(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRuiDad->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


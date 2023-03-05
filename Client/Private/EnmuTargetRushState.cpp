#include "stdafx.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuTargetRushState.h"
#include "EnmuIdleState.h"
#include "Camera_Dynamic.h"
#include "SoundMgr.h"
#include "Layer.h"
using namespace Enmu;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CTargetRushState::HandleInput(CEnmu* pEnmu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CEnmuState * CTargetRushState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Transform()->Set_PlayerLookAt(pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START: 
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CEnmuState::TYPE_LOOP: 
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());;
			break;
		case Client::CEnmuState::TYPE_END: 
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_LOOP:
		Move(pEnmu, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CEnmuState * CTargetRushState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pEnmu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	switch (m_eStateType)
	{
	case CEnmuState::TYPE_START:
		if (!m_bEffect)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_START, pEnmu);
			m_bEffect = true;
		}
		break;
	case CEnmuState::TYPE_LOOP:
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_MOVE, pEnmu);
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CEffect_Manager);

	return nullptr;
}

void CTargetRushState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_RUSH;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_TARGET_RUSH_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_TARGET_RUSH_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_TARGET_RUSH_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pEnmu->Get_Transform()->Set_PlayerLookAt(pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Dash.wav"), g_fEffect);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_TARGET_RUSH_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_TARGET_RUSH_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pEnmu);
		break;
	case Client::CEnmuState::TYPE_END:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_TARGET_RUSH_2);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_TARGET_RUSH_2);
		//pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_TARGET_RUSH_1, false);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_TARGET_RUSH_2);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CEnmu* pEnmu)
{
}

void CTargetRushState::Move(CEnmu* pEnmu, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, pEnmu->Get_NavigationHeight().y + 0.1f, m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pEnmu->Get_SphereCollider()->Collision(pEnmu->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pEnmu->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pEnmu->Get_BattleTarget()->Get_PlayerInfo().bGuard && pEnmu->Get_BattleTarget()->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			pEnmu->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUSH_HIT, pEnmu);
			RELEASE_INSTANCE(CEffect_Manager);
			pEnmu->Get_BattleTarget()->Take_Damage(0.3f, false);
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
			
			RELEASE_INSTANCE(CGameInstance);

			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pEnmu->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
		}
	}
	else
		pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CEnmu* pEnmu)
{
	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pEnmu->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}


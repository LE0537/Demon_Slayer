#include "stdafx.h"
#include "NezukoGuardAdvState.h"
#include "NezukoIdleState.h"
#include "NezukoGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Nezuko;


CGuardAdvState::CGuardAdvState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CNezukoState * CGuardAdvState::HandleInput(CNezuko* pNezuko)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_GUARD_ADV))
	{
		pNezuko->Get_Model()->Set_End(CNezuko::ANIM_GUARD_ADV);

		switch (pNezuko->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_O) && pNezuko->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_C) && pNezuko->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
			break;
		default:
			break;
		}
	
	}


	return nullptr;
}

CNezukoState * CGuardAdvState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_bGuard(true);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_ADV);
	pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_ADV, 0.02f);


	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CNezukoState * CGuardAdvState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pNezuko->Get_Transform()->LookAt(vLooAt);

	_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
	_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
	vCollPos += XMVector3Normalize(vCollLook) * 1.5f; //추가
	vCollPos.m128_f32[1] = 1.f; //추가
	m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
	CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	m_fMove += fTimeDelta;
	if (m_fMove > 0.1f && !m_bHit)
	{
		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			
			//pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, vTagetWorld);

			RELEASE_INSTANCE(CEffect_Manager);

			m_bHit = true;
		}

	}
	if (m_fMove < 0.3f && m_bHit)
	{
		_vector vTagetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vTagetPos += XMVector3Normalize(vPos) * 40.f * fTimeDelta;
		vTagetPos.m128_f32[1] = 0.f;
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTagetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTagetPos);
	}

	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardAdvState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_GUARD_ADV);
	pNezuko->Set_AnimIndex(CNezuko::ANIM_GUARD_ADV);

	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_GuardAdv.wav"), fEFFECT);
}

void CGuardAdvState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead(); //추가
}


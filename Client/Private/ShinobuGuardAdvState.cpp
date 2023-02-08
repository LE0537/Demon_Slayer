#include "stdafx.h"
#include "ShinobuGuardAdvState.h"
#include "ShinobuIdleState.h"
#include "ShinobuGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Shinobu;


CGuardAdvState::CGuardAdvState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CShinobuState * CGuardAdvState::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pShinobu->Get_i1P())
	{
	case 1:
		if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_GUARD_ADV))
		{
			pShinobu->Get_Model()->Set_End(CShinobu::ANIM_GUARD_ADV);

			if (pGameInstance->Key_Pressing(DIK_O))
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
		}
		break;
	case 2:
		if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_GUARD_ADV))
		{
			pShinobu->Get_Model()->Set_End(CShinobu::ANIM_GUARD_ADV);

			if (pGameInstance->Key_Pressing(DIK_C))
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
			else
				return new CGuardState(STATE_TYPE::TYPE_END);
		}
		break;
	default:
		break;
	}

	return nullptr;
}

CShinobuState * CGuardAdvState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_bGuard(true);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_GUARD_ADV);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_GUARD_ADV, 0.02f);


	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CShinobuState * CGuardAdvState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pShinobu->Get_Transform()->LookAt(vLooAt);

	_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
	_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
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
			_int iDest = rand() % 5;
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			switch (iDest)
			{
			case 0:
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
				break;
			case 1:
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
				break;
			case 2:
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
				break;
			case 3:
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
				break;
			case 4:
				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
				break;
			default:
				break;
			}


			RELEASE_INSTANCE(CEffect_Manager);

			m_bHit = true;
		}

	}
	if (m_fMove < 0.3f && m_bHit)
	{
		_vector vTagetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vTagetPos += XMVector3Normalize(vPos) * 40.f * fTimeDelta;
		vTagetPos.m128_f32[1] = 0.f;
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTagetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTagetPos);
	}

	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardAdvState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_GUARD_ADV);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead(); //추가
}


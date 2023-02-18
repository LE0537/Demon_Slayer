#include "stdafx.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Rui;

CAdvSkill_MoveState::CAdvSkill_MoveState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiMoveSkill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiState * CAdvSkill_MoveState::HandleInput(CRui * pRui)
{
	return nullptr;
}

CRuiState * CAdvSkill_MoveState::Tick(CRui * pRui, _float fTimeDelta)
{
	m_fBackStepTime += fTimeDelta;

	if (m_fBackStepTime <= 0.2f)
		pRui->Get_Transform()->Go_Backward(fTimeDelta *  1.2f, pRui->Get_NavigationCom());


	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
		return new CIdleState();
	}

	return nullptr;
}

CRuiState * CAdvSkill_MoveState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	if (m_fBackStepTime > 0.2f)
	{
		CCharacters* m_pTarget = pRui->Get_BattleTarget();
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vLooAt.m128_f32[1] = 0.f;
		pRui->Get_Transform()->LookAt(vLooAt);

			m_fMove += fTimeDelta;


			_vector vCollPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 4.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pRui->Get_SphereCollider();
			if (m_fMove > 0.2f && m_iHit == 0)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-150 * pRui->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if(pRui->Get_BattleTarget()->Get_GodMode() == false)
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.4f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(_int(-80 * pRui->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.3f, true);
						pRui->Set_Combo(1);
						pRui->Set_ComboTime(0.f);
					}
					if (pRui->Get_BattleTarget()->Get_GodMode() == false)
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

						++m_iHit;
					}
				}
			}
		
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta * 0.8f);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDMOVE, pRui);
	
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAdvSkill_MoveState::Enter(CRui * pRui)
{
	m_eStateId = STATE_ID::STATE_ADVSKILL_COMMON;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_ADVSKILL_MOVE);
	pRui->Set_AnimIndex(CRui::ANIM_ADVSKILL_MOVE);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_ADVSKILL_MOVE);
	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_FriendAdvMove.wav"), fEFFECT);
}

void CAdvSkill_MoveState::Exit(CRui * pRui)
{
	m_pCollBox->Set_Dead(); //추가
}



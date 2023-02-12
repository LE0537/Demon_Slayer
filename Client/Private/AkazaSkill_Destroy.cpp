#include "stdafx.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaDashState.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaAdvSkill_Move.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaTargetRushState.h"
#include "AkazaJumpState.h"
#include "AkazaAtk_1_State.h"
#include "Camera_Dynamic.h"
using namespace Akaza;


CSkill_DestoryState::CSkill_DestoryState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CAkazaState * CSkill_DestoryState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIM_SKILL_DESTROY_1);
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIM_SKILL_DESTROY_1);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.6f)
	{
		switch (pAkaza->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_W)) // 앞
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RF);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_STRAIGHT);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_S)) // 뒤
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우 
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RB);

				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_BACK);
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // 좌
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 우
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_UP)) // 앞
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RF);


				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_STRAIGHT);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 뒤
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우 
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RB);

				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_BACK);

				}
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}
	return CommandCheck(pAkaza);
}

CAkazaState * CSkill_DestoryState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_float fDist = 0.f;

	if (m_eStateType == CAkazaState::TYPE_START)
	{
		_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vLook2 = vPos - vTarget;

		_float fDist = XMVectorGetX(XMVector3Length(vLook2));

		if (fDist < 18.f)
		{
			if (pAkaza->Get_Model()->Get_CurrentTime() > 30.f)
			{
				pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
				_vector vTargetPos = vPos - vTarget;

				vTarget += XMVector3Normalize(vTargetPos) * 2.f;
				vTarget.m128_f32[1] = 0.f;
				if(pAkaza->Get_NavigationCom()->Cheak_Cell(vTarget))
					pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTarget);
				//pAkaza->Get_NavigationCom()->Find_CurrentCellIndex(vTarget);
				return new CSkill_DestoryState(CAkazaState::TYPE_END);
			}
		}
	}

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;
}

CAkazaState * CSkill_DestoryState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

	if (m_eStateType == CAkazaState::TYPE_END)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		if (m_fMove > 0.15f && m_fMove < 0.45f)
		{
			if (m_iHit < 1)
			{
				_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				vCollPos += XMVector3Normalize(vCollLook) * 2.f; //추가
				vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_float4 vTagetPos;
					XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					vPos.m128_f32[1] = 0.f;
					m_pTarget->Get_Transform()->LookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-50 * pAkaza->Get_PlayerInfo().fPowerUp);
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(-20 * pAkaza->Get_PlayerInfo().fPowerUp);
						m_pTarget->Take_Damage(0.1f, false);
						pAkaza->Set_Combo(1);
						pAkaza->Set_ComboTime(0.f);
					}

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


			CCollider*	pMyCollider = pAkaza->Get_SphereCollider();
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				_vector vPlayerPosY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
					pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pAkaza->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pAkaza->Get_NavigationCom());
			}
		}
		else if (m_fMove > 0.5f && m_fMove < 1.f)
		{
			if (!m_bHit)
			{
				_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				vCollPos += XMVector3Normalize(vCollLook) * 4.f; //추가
				vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-50 * pAkaza->Get_PlayerInfo().fPowerUp);
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.2f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(-30 * pAkaza->Get_PlayerInfo().fPowerUp);
						m_pTarget->Take_Damage(0.5f, true);
						pAkaza->Set_Combo(1);
						pAkaza->Set_ComboTime(0.f);
					}

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
		}

		RELEASE_INSTANCE(CGameInstance);
	}
	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);
		break;
	}

	if (!m_bEffect && m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_SPECIALGROUND, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_DESTROY_MAIN, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	else if (!m_bEffect && m_eStateType == TYPE_END)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		if (true == m_bHit)
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_DESTROY_SUCCESSS_FINAL, pAkaza);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_DESTROY_FAILEDFINAL, pAkaza);
		}
		else
		{
			pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_DESTROY_FAILEDFINAL, pAkaza);
		}

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CSkill_DestoryState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_DESTROY;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_DESTROY_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_DESTROY_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_DESTROY_0);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_Destroy.wav"), fEFFECT);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_DESTROY_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_DESTROY_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_DESTROY_1);
		break;
	}

}

void CSkill_DestoryState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}

CAkazaState * CSkill_DestoryState::CommandCheck(CAkaza * pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIM_SKILL_DESTROY_1);
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIM_SKILL_DESTROY_1);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
	{
		switch (pAkaza->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();


			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pAkaza->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						pAkaza->Set_SkillBar(-200);
						return new CSkill_DestoryState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pAkaza->Set_SkillBar(-200);
						return new CSkill_PunchState(TYPE_START); // move skill
					}
					else
					{
						pAkaza->Set_SkillBar(-200);
						return new CSkill_ShootState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_L))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_SPACE))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_U))
			{
				if (pAkaza->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_MoveState(TYPE_START);
					}
					else
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommmonState(TYPE_START);
					}
				}
			}

			break;
		case 2:
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pAkaza->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						pAkaza->Set_SkillBar(-200);
						return new CSkill_DestoryState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						pAkaza->Set_SkillBar(-200);
						return new CSkill_PunchState(TYPE_START); // move skill
					}
					else
					{
						pAkaza->Set_SkillBar(-200);
						return new CSkill_ShootState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_LCONTROL))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_V))
			{
				if (pAkaza->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_MoveState(TYPE_START);
					}
					else
					{
						pAkaza->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommmonState(TYPE_START);
					}
				}
			}
		}
	}


	return nullptr;
}


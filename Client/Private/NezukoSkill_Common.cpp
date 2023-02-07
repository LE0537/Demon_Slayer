#include "stdafx.h"
#include "NezukoSkill_Common.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "NezukoDashState.h"
#include "NezukoSkill_Common.h"
#include "NezukoSkill_FallCut.h"
#include "NezukoSkill_Move.h"
#include "NezukojumpState.h"
#include "NezukoTargetRushState.h"
#include "NezukoAtk_1_State.h"
#include "Camera_Dynamic.h"
using namespace Nezuko;


CSkill_CommonState::CSkill_CommonState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
	m_fMove = 0.15f;
}

CNezukoState * CSkill_CommonState::HandleInput(CNezuko* pNezuko)
{
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pNezuko->Get_Model()->Get_Duration_Index(CNezuko::ANIM_SKILL_COMMON_2);
	m_fCurrentDuration = pNezuko->Get_Model()->Get_CurrentTime_Index(CNezuko::ANIM_SKILL_COMMON_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
	{
		switch (pNezuko->Get_i1P())
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


	return CommandCheck(pNezuko);
}

CNezukoState * CSkill_CommonState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
	m_fComboDelay += fTimeDelta * 60;
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_CommonState(CNezukoState::TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_CommonState(CNezukoState::TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	return nullptr;
}

CNezukoState * CSkill_CommonState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pNezuko->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}

	m_fMove += fTimeDelta;

	if (m_eStateType == TYPE_START)
	{
		pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pNezuko->Get_NavigationCom());
		CCollider*	pMyCollider = pNezuko->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPos))
				pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pNezuko->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pNezuko->Get_NavigationCom());
		}
	}
	if (m_eStateType == TYPE_LOOP)
	{
		if (m_fMove > 0.15f)
		{
			pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pNezuko->Get_NavigationCom());

			if (m_iHit < 6)
			{
				_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
				vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.1f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(-15);
						m_pTarget->Take_Damage(0.f, false);
						pNezuko->Set_Combo(1);
						pNezuko->Set_ComboTime(0.f);
					}

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

					RELEASE_INSTANCE(CEffect_Manager);

					++m_iHit;
					m_fMove = 0.f;
				}
			}


			CCollider*	pMyCollider = pNezuko->Get_SphereCollider();
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float fSpeed = pNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPos))
					pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pNezuko->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pNezuko->Get_NavigationCom());
			}
		}
	}
	if (m_eStateType == TYPE_END)
	{
		if (m_fMove > 0.4f)
		{
			if (!m_bHit)
			{
				_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
				_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
				vCollPos += XMVector3Normalize(vCollLook) * 4.f; //추가
				vCollPos.m128_f32[1] = 1.f; //추가
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						m_pTarget->Set_Hp(-30);
						m_pTarget->Take_Damage(0.5f, false);
						pNezuko->Set_Combo(1);
						pNezuko->Set_ComboTime(0.f);
					}

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

					RELEASE_INSTANCE(CEffect_Manager);

					m_bHit = true;
				}
			}
		}
	}
	if (m_eStateType == TYPE_LOOP)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.5f);

	return nullptr;
}

CNezukoState * CSkill_CommonState::CommandCheck(CNezuko * pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pNezuko->Get_Model()->Get_Duration_Index(CNezuko::ANIM_SKILL_COMMON_2);
	m_fCurrentDuration = pNezuko->Get_Model()->Get_CurrentTime_Index(CNezuko::ANIM_SKILL_COMMON_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
	{
		switch (pNezuko->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pNezuko->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pNezuko->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill
					}
					else
					{
						pNezuko->Set_SkillBar(-200);
						return new CSkill_CommonState(TYPE_START);
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
			break;
		case 2:
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pNezuko->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{


						pNezuko->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill

					}
					else
					{
						pNezuko->Set_SkillBar(-200);
						return new CSkill_CommonState(TYPE_START);
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
			break;
		}

	}

	return nullptr;
}

void CSkill_CommonState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_COMMON_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_COMMON_0, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_COMMON_0);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_COMMON_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_COMMON_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_COMMON_1);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_COMMON_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_COMMON_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_COMMON_2);
		break;
	}

}

void CSkill_CommonState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}


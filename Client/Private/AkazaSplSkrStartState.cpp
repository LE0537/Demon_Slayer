#include "stdafx.h"
#include "..\Public\AkazaSplSkrStartState.h"
#include "GameInstance.h"
#include "Akaza_CinemaState.h"
#include "AkazaIdleState.h"
#include "Effect_Manager.h"
using namespace Akaza;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CAkazaState * CSplSkrStartState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CSplSkrStartState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				pAkaza->Get_BattleTarget()->Play_Scene();
				return new CAkaza_CinemaState(CAkaza_CinemaState::SCENE_START);
			}
			else
			{
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CAkazaState::TYPE_END:
			break;
		case Client::CAkazaState::TYPE_DEFAULT:
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		break;
	case Client::CAkazaState::TYPE_LOOP:
		if (pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIMID::ANIM_SPLSKL_START_1) <= 10.f && pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIMID::ANIM_SPLSKL_START_1) >= 2.f)
		{
			m_fDuration += fTimeDelta;

			if (m_fDuration <= 0.5f)
				Move(pAkaza, fTimeDelta);
		}
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CAkazaState * CSplSkrStartState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (m_eStateType == CAkazaState::TYPE_LOOP)
	{
		CCharacters* m_pTarget = pAkaza->Get_BattleTarget();

		m_fMove += fTimeDelta;

		if (m_fMove > 0.3f)
		{
			_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pAkaza->Get_SphereCollider();
			if (m_fMove < 0.5f && !m_bHit)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-1);
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pAkaza->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_bCollision = true;
					}
					if (pAkaza->Get_BattleTarget()->Get_GodMode() == false)
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

			}


			if (pMyCollider2->Collision(pTargetCollider))
			{
				_float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				if (g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU)
					vPos.m128_f32[1] = 0.f;
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
	}
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SPLSKL_START_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SPLSKL_START_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_SPLSKL_START_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SPLSKL_START_0, 0.01f);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SPLSKL_START_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SPLSKL_START_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_SPLSKL_START_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SPLSKL_START_1, 0.01f);
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}

void CSplSkrStartState::Move(CAkaza* pAkaza, _float fTimeDelta)
{
	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
	pAkaza->Get_Transform()->Go_Straight(fTimeDelta * 2.2f);
}

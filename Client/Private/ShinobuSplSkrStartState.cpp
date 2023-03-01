#include "stdafx.h"
#include "..\Public\ShinobuSplSkrStartState.h"
#include "GameInstance.h"
#include "Shinobu_CinemaState.h"
#include "ShinobuIdleState.h"
#include "Effect_Manager.h"
using namespace Shinobu;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CShinobuState * CSplSkrStartState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CSplSkrStartState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_LOOP:
			if (m_bCollision == true)
			{
				pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				pShinobu->Get_BattleTarget()->Play_Scene();
				return new CShinobu_CinemaState(CShinobu_CinemaState::SCENE_START);
			}
			else
			{
				pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				return new CIdleState();
			}
			break;
			break;
		case Client::CShinobuState::TYPE_END:
			break;
		case Client::CShinobuState::TYPE_DEFAULT:
			break;
		case Client::CShinobuState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		break;
	case Client::CShinobuState::TYPE_LOOP:
		if (pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIMID::ANIM_SPLSKL_START_1) >= 10.f)
		{
			m_fDuration += fTimeDelta;

			if (m_fDuration <= 0.5f)
				Move(pShinobu, fTimeDelta);
		}

		break;
	case Client::CShinobuState::TYPE_END:

	case Client::CShinobuState::TYPE_DEFAULT:
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CShinobuState * CSplSkrStartState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == CShinobuState::TYPE_LOOP)
	{
		CCharacters* m_pTarget = pShinobu->Get_BattleTarget();

		m_fMove += fTimeDelta;

		if (m_fMove > 0.3f)
		{
			_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
			if (m_fMove < 0.5f && !m_bHit)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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
					else if (pShinobu->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_bCollision = true;
					}
					if (pShinobu->Get_BattleTarget()->Get_GodMode() == false)
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
				_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				if (g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU)
					vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
					pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pShinobu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pShinobu->Get_NavigationCom());
			}
		}
	}

	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_SPLSKL_START_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SPLSKL_START_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SPLSKL_START_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SPLSKL_START_0, 0.1f);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_SPLSKL_START_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SPLSKL_START_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SPLSKL_START_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SPLSKL_START_1, 0.1f);
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

void CSplSkrStartState::Move(CShinobu* pShinobu, _float fTimeDelta)
{
	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}

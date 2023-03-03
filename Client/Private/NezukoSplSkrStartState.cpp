#include "stdafx.h"
#include "..\Public\NezukoSplSkrStartState.h"
#include "GameInstance.h"
#include "Nezuko_CinemaState.h"
#include "NezukoIdleState.h"
#include "Effect_Manager.h"
using namespace Nezuko;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CNezukoState * CSplSkrStartState::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CSplSkrStartState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSplSkrStartState(TYPE_DEFAULT);
			break;
		case Client::CNezukoState::TYPE_DEFAULT:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CNezukoState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		break;
	case Client::CNezukoState::TYPE_LOOP:
		break;
	case Client::CNezukoState::TYPE_END:
		m_fDuration += fTimeDelta;

		if (m_fDuration <= 0.7f)
			Move(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		if (m_bCollision == true)
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			pNezuko->Get_BattleTarget()->Play_Scene();
			return new CNezuko_CinemaState(CNezuko_CinemaState::SCENE_START);
		}
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CNezukoState * CSplSkrStartState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (m_eStateType == CNezukoState::TYPE_DEFAULT)
	{
		CCharacters* m_pTarget = pNezuko->Get_BattleTarget();


		_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] += 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pNezuko->Get_SphereCollider();
		if (m_fMove < 0.2f && !m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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
				else if (pNezuko->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_bCollision = true;
				}
				if (pNezuko->Get_BattleTarget()->Get_GodMode() == false)
				{
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_Hit_SplSkr.wav"), g_fEffect);
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
			_float fSpeed = pNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			if (g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU)
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


	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_0);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_0, 0.01f);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Nezuko_SplSkr_Start.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_SplSkr_Start.wav"), g_fEffect);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_1, 0.01f);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_2);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_2, 0.01f);
		break;
	case Client::CNezukoState::TYPE_DEFAULT:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SPLSKL_START_3);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SPLSKL_START_3);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_START_3);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_START_3, 0.01f);
		break;
	case Client::CNezukoState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

void CSplSkrStartState::Move(CNezuko* pNezuko, _float fTimeDelta)
{
	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	if (fDistance >= 5.f)
		pNezuko->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}

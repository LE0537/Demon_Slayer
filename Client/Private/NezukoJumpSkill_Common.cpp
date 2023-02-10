#include "stdafx.h"
#include "NezukoJumpSkill_Common.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "NezukojumpState.h"
#include "Camera_Dynamic.h"
using namespace Nezuko;


CJumpSkill_CommmonState::CJumpSkill_CommmonState(STATE_TYPE eType, _uint iCount)
	: m_iCount(iCount)
{

	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WindMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);

}

CNezukoState * CJumpSkill_CommmonState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	

	return nullptr;
}

CNezukoState * CJumpSkill_CommmonState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CJumpSkill_CommmonState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CJumpSkill_CommmonState(TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			break;
		}
	}


	if (TYPE_END == m_eStateType)
	{
	

		if (pNezuko->Get_Model()->Get_CurrentTime() >=30.f)
		{
			Jump(pNezuko, fTimeDelta);

		
		}


		if (pNezuko->Get_Model()->Get_CurrentTime() >= 40.f)
			return new CJumpState(TYPE_END, 0.f,0.f);
	}
	

	

	return nullptr;
}

CNezukoState * CJumpSkill_CommmonState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pNezuko->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}

	m_fDelay += fTimeDelta;
	
	m_fTime += fTimeDelta;
	m_fHitTime += fTimeDelta;
	

		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pNezuko->Get_SphereCollider();
		if (m_eStateType == Client::CNezukoState::TYPE_START)
		{
			_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
																							 //m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

			if (m_fHitTime > 0.10f && m_iHit < 2)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-15 * pNezuko->Get_PlayerInfo().fPowerUp);
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
						m_pTarget->Set_Hp(-10 * pNezuko->Get_PlayerInfo().fPowerUp);
						m_pTarget->Take_Damage(0.f, false);
						pNezuko->Set_Combo(1);
						pNezuko->Set_ComboTime(0.f);
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

					m_fHitTime = 0.f;
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
			_vector vPlayerPosY = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
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
 		if (m_eStateType == Client::CNezukoState::TYPE_LOOP)
		{
			_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
																							 //m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

			if (m_fHitTime > 0.3f && m_iHit < 3)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-15 * pNezuko->Get_PlayerInfo().fPowerUp);
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
						m_pTarget->Set_Hp(-10 * pNezuko->Get_PlayerInfo().fPowerUp);
						m_pTarget->Take_Damage(0.f, false);
						pNezuko->Set_Combo(1);
						pNezuko->Set_ComboTime(0.f);
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

					m_fHitTime = 0.f;
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
				_vector vPlayerPosY = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
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
		if (m_eStateType == Client::CNezukoState::TYPE_END)
		{
			_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
																							 //m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

			if (m_fHitTime > 0.3f && m_iHit < 2 && m_fDelay < 0.7f)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-15 * pNezuko->Get_PlayerInfo().fPowerUp);
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
						m_pTarget->Set_Hp(-10 * pNezuko->Get_PlayerInfo().fPowerUp);
						m_pTarget->Take_Damage(0.f, false);
						pNezuko->Set_Combo(1);
						pNezuko->Set_ComboTime(0.f);
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

					m_fHitTime = 0.f;
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
				_vector vPlayerPosY = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
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

void CJumpSkill_CommmonState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_JUMP;

	_uint iRand = rand() % 3;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_JUMPCOMMON_0);
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_Common1.wav"), fEFFECT);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_Common2.wav"), fEFFECT);
		else if (iRand == 2)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_Common3.wav"), fEFFECT);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_JUMPCOMMON_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_1, false);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_JUMPCOMMON_2);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		Initialize_value(pNezuko);
		if (iRand == 0)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_Common1_1.wav"), fEFFECT);
		else if (iRand == 1)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_Common2_1.wav"), fEFFECT);
		else if (iRand == 2)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_Common3_1.wav"), fEFFECT);
		break;
	}

}

void CJumpSkill_CommmonState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

void CJumpSkill_CommmonState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);

	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;



	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		pNezuko->Get_Transform()->Set_Jump(false);
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
			 
			m_bNextAnim = true;
			//pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_END);
			//pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
			//pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_END, false);
			//pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		}
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}

}

void CJumpSkill_CommmonState::Initialize_value(CNezuko* pNezuko)
{

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}



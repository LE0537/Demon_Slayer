#include "stdafx.h"
#include "NezukoSkill_FallCut.h"
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
#include "NezukoSplSkrStartState.h"
using namespace Nezuko;


CSkill_FallCutState::CSkill_FallCutState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;

}

CNezukoState * CSkill_FallCutState::HandleInput(CNezuko* pNezuko)
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

CNezukoState * CSkill_FallCutState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_EffectTime(fTimeDelta);
	m_fComboDelay += fTimeDelta * 60;

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_FallCutState(CNezukoState::TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (m_bNextAnim == true)
			{		
				return new CSkill_FallCutState(CNezukoState::TYPE_END);
			}

			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			if (vPos.m128_f32[1] > 0.f)
			{
				vPos.m128_f32[1] = 0.f;
				pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
			pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}


	if (m_eStateType == STATE_TYPE::TYPE_START)
		Increase_Height(pNezuko, fTimeDelta);
	if (m_eStateType == STATE_TYPE::TYPE_LOOP)
		Fall_Height(pNezuko, fTimeDelta);

	return nullptr;
}

CNezukoState * CSkill_FallCutState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();

	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pNezuko->Get_Transform()->LookAt(vLooAt);
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pNezuko->Get_SphereCollider();
	m_fMove += fTimeDelta;

	/*if (m_eStateType == STATE_TYPE::TYPE_START)
	{
		if(!m_bLook)
			pNezuko->Get_Transform()->Go_Straight(fTimeDelta, pNezuko->Get_NavigationCom());
	}*/
	if (m_eStateType == STATE_TYPE::TYPE_LOOP)
	{
		_vector vCollPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 2.5f; //추가
		vCollPos.m128_f32[1] = 0.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
	
		if (!m_bHit)
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
					m_pTarget->Set_GuardHp(_int(-150 * pNezuko->Get_PlayerInfo().fPowerUp));
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
					m_pTarget->Set_Hp(_int(-80 * pNezuko->Get_PlayerInfo().fPowerUp));
					m_pTarget->Player_UpperDown(CCharacters::HIT_TYPE::HIT_KNOCKBACK, 20.f, 30.f, 5.f);

					CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
					dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pNezuko->Get_Renderer());
					RELEASE_INSTANCE(CGameInstance);

					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_Hit_JumpMoveAttack_0.wav"), g_fEffect);
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

				m_bHit = true;
			}
		}
	}
	/*if (pMyCollider2->Collision(pTargetCollider))
	{
		_float fSpeed = pNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 2.f;

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
	}*/

	if (!m_bLook)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 2.f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	if (m_eStateType == STATE_TYPE::TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_NEZUKO_JUMP_KICK, pNezuko);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	if (m_eStateType == STATE_TYPE::TYPE_END)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_NEZUKO_JUMP_KICK_GROUND, pNezuko);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}

	return nullptr;
}

void CSkill_FallCutState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_FALLCUT;



	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		_vector vLook = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		pNezuko->Get_Transform()->Set_PlayerLookAt(vLook);

		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_0, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_JumpMoveAttack.wav"), g_fEffect);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		if (!m_bLookPos)
		{
			_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		/*	_float fDist = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
			if (fDist < 10.f)
				m_bLook = true;*/
			m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
			m_bLookPos = true;
		}
		m_fGravity = 15.81f;
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_FallCut.wav"), g_fEffect);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_2);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
	/*	if (!m_bLookPos)
		{
			_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fDist = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
			if (fDist < 10.f)
				m_bLook = true;
			m_bLookPos = true;
		}*/
		break;
	}

}

void CSkill_FallCutState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

CNezukoState * CSkill_FallCutState::Increase_Height(CNezuko * pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 100.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 7.f)
	{

		//pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPosition))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}


	return nullptr;
}

CNezukoState * CSkill_FallCutState::Fall_Height(CNezuko * pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);
	static _float fGravity = -500.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity * fTimeDelta;
	if (!m_bLook)
	{
		vPosition.x += XMVectorGetX(m_vTargetPosition) * 50.f * fTimeDelta;
		vPosition.y += XMVectorGetY(m_vTargetPosition) * -fVelocity * fTimeDelta;
		vPosition.z += XMVectorGetZ(m_vTargetPosition) * 50.f * fTimeDelta;
	}
	else
		vPosition.y += XMVectorGetY(m_vTargetPosition) * -fVelocity * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	if (vPosition.y <= m_fOriginPosY)
	{
		pNezuko->Get_Transform()->Set_Jump(false);
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, vPosition.x);
		vecPos = XMVectorSetY(vecPos, vPosition.y);
		vecPos = XMVectorSetZ(vecPos, vPosition.z);
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		else
		{
			vecPos = XMVectorSetX(vecPos, XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			vecPos = XMVectorSetZ(vecPos, XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		}
		m_bNextAnim = true;
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		else
		{
			vecPos = XMVectorSetX(vecPos, XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			vecPos = XMVectorSetZ(vecPos, XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		}
	}

	return nullptr;
}

CNezukoState * CSkill_FallCutState::CommandCheck(CNezuko * pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pNezuko->Get_Model()->Get_Duration_Index(CNezuko::ANIM_SKILL_FALLCUT_2);
	m_fCurrentDuration = pNezuko->Get_Model()->Get_CurrentTime_Index(CNezuko::ANIM_SKILL_FALLCUT_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.5f)
	{
		switch (pNezuko->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pNezuko->Get_PlayerInfo().iUnicCount > 0)
				{
					pNezuko->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pNezuko->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
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
			if (pGameInstance->Key_Down(DIK_RSHIFT))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pNezuko->Get_PlayerInfo().iUnicCount > 0)
				{
					pNezuko->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pNezuko->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);

						pNezuko->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill

					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
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

